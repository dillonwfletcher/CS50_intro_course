import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # grab all user purchases from purchases table in db
    rows = db.execute("SELECT symbol, total_shares FROM inventory WHERE user_id = :user ORDER BY symbol ASC", user=session['user_id'])

    # select amount of cash user has available from db using session id
    user_cash = db.execute("SELECT cash FROM users WHERE id = :user", user=session["user_id"])

    # create a dictinoary to store data for owned companies
    owned = {}

    # to keep track of total value of stocks owned
    total_value = 0

    # insert data into owned companies dictionary
    for company in rows:

        # get data for owned company using IEX api
        quote = lookup(company['symbol'])

        # set dictionary index to company symbol and store a dictionary of data for company at index
        owned[company['symbol']] = {'name': quote['name'], 'price': usd(quote['price']), 'value': usd(quote['price'] * company['total_shares'])}

        # add value of stocks for this company to total value
        total_value += (quote['price'] * company['total_shares'])

    return render_template("index.html", companies=rows, owned=owned, cash=usd(user_cash[0]['cash']), value=usd(total_value + user_cash[0]['cash']))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # lookup ticker using IEX API and store quote data in dictionary
        quote = lookup(request.form.get("ticker"))

        # ensure that lookup worked and that company exists
        if not quote:
            return apology("company does not exist", 403)

        # select amount of cash user has available from db using session id
        user_cash = db.execute("SELECT cash FROM users WHERE id = :user", user=session["user_id"])

        # calculate cost of stock purchase; result of type float
        purchase_cost = quote["price"] * float(request.form.get("shares"))

        # ensure that user has enough cash to make purchase
        if purchase_cost > user_cash[0]['cash']:
            return apology("you do not have enough cash available to complete this purchase", 403)

        # initate purchase and insert purchase data into purchases table and transactions table
        db.execute("INSERT INTO purchases ('user_id','symbol', 'purchase_price', 'shares_purchased', 'purchase_date') VALUES (:user, :symbol, :price, :shares, datetime('now', 'localtime'))",
                    user=session["user_id"], symbol=quote['symbol'], price=quote["price"], shares=request.form.get("shares"))

        db.execute("INSERT INTO transactions ('user_id', 'type', 'symbol', 'company', 'shares', 'price', 'transaction_date') VALUES (:user, :ttype, :symbol, :company, :shares, :price, datetime('now', 'localtime'))",
                    user=session["user_id"], ttype="BUY", symbol=quote['symbol'], company=quote['name'], shares=request.form.get("shares"), price=quote["price"])

        # check to see if user already owns stock
        inventory = db.execute("SELECT avg_price, total_shares FROM inventory WHERE user_id = :user AND symbol = :symbol",
                                user=session["user_id"], symbol=quote['symbol'])

        # if user does not own stock then update inventory to recognize purchase of stock
        if len(inventory) == 0:
            db.execute("INSERT INTO inventory ('user_id','symbol', 'avg_price', 'total_shares') VALUES (:user, :symbol, :price, :shares)",
                    user=session["user_id"], symbol=quote['symbol'], price=quote["price"], shares=request.form.get("shares"))

        # if user already owns stock then update current inventory of stock with new purchase
        else:
            total_shares = inventory[0]['total_shares'] + int(request.form.get("shares"))
            avg_price = (inventory[0]['avg_price'] * inventory[0]['total_shares'] + quote['price'] * float(request.form.get("shares"))) / total_shares
            db.execute("UPDATE inventory SET avg_price=:avg_price, total_shares=:shares WHERE user_id = :user AND symbol = :symbol",
                        avg_price=avg_price, shares=total_shares, user=session["user_id"], symbol=quote['symbol'])

        # update users cash in users table in db
        new_cash = user_cash[0]['cash'] - purchase_cost
        db.execute("UPDATE users SET cash = :new_cash WHERE id = :user", new_cash=new_cash, user=session["user_id"])

        #TODO add alert on this page to confirm purchase
        return redirect("/")

    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    transactions = db.execute("SELECT * FROM transactions WHERE user_id = :user ORDER BY transaction_date DESC", user=session["user_id"])

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # lookup ticker using IEX API and store quote data in dictionary
        quote = lookup(request.form.get("ticker"))

        # ensure ticker existed and that we were able to pull data using api
        if not quote:
            return apology("company does not exist", 403)

        # if ticker exists display quote information to user
        return render_template("quoted.html", name=quote["name"], price=usd(quote["price"]), symbol=quote["symbol"])

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password") or not request.form.get("confirmation"):
            return apology("must provide password", 403)

        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords must match", 403)

        # Query database for username to ensure that username does not already exist
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        if len(rows) != 0:
            return apology("username already exists", 403)

        # new user registration successful: hash password and insert new user into database
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hashed_password)",
                    username=request.form.get("username"), hashed_password=generate_password_hash(request.form.get("password")))

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure a symbol owned was selected
        if not request.form.get("symbol"):
            return apology("no company selected", 403)

        # query inventory table
        inventory = db.execute("SELECT avg_price, total_shares FROM inventory WHERE user_id = :user AND symbol = :symbol",
                                user=session["user_id"], symbol=request.form.get("symbol"))

        # ensure user is not trying to sell more shares than they own
        if int(request.form.get("shares")) > inventory[0]['total_shares']:
            return apology("you cannot sell more shares than you own", 403)

        quote = lookup(request.form.get("symbol"))

        # initate sale and insert sale data into sales and transactions table in db
        db.execute("INSERT INTO sales ('user_id','symbol', 'sale_price', 'shares_sold', 'sale_date') VALUES (:user, :symbol, :price, :shares, datetime('now', 'localtime'))",
                    user=session["user_id"],
                    symbol=request.form.get("symbol"),
                    price=quote['price'],
                    shares=request.form.get("shares"))

        db.execute("INSERT INTO transactions ('user_id', 'type', 'symbol', 'company', 'shares', 'price', 'transaction_date') VALUES (:user, :ttype, :symbol, :company, :shares, :price, datetime('now', 'localtime'))",
                    user=session["user_id"], ttype="SELL", symbol=quote['symbol'], company=quote['name'], shares=request.form.get("shares"), price=quote["price"])

        # calculate profit from sale
        #profit = (quote['price'] - inventory[0]['avg_price']) * float(request.form.get("shares"))

        # calculate cash from sale
        sale_cash = quote['price'] * float(request.form.get("shares"))

        # update inventory
        new_total_shares = inventory[0]['total_shares'] - int(request.form.get("shares"))

        # if user did not sell all their shares
        if new_total_shares != 0:

            # then update inventory to represent new balance
            db.execute("UPDATE inventory SET total_shares = :shares WHERE user_id = :user AND symbol = :symbol",
                        shares=new_total_shares, user=session["user_id"], symbol=request.form.get("symbol"))

        # if user sold all their shares
        else:

            # then delete record of stock from inventory since share amount is now zero
            db.execute("DELETE FROM inventory WHERE user_id = :user AND symbol=:symbol",
                        user=session["user_id"], symbol=request.form.get("symbol"))

        # update users cash in users table in db
        user_cash = db.execute("SELECT cash FROM users WHERE id = :user", user=session["user_id"])
        new_cash = user_cash[0]['cash'] + sale_cash
        db.execute("UPDATE users SET cash = :new_cash WHERE id = :user", new_cash=new_cash, user=session["user_id"])

        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:

        # grab all stocks owned by user
        rows = db.execute("SELECT symbol FROM inventory WHERE user_id = :user ORDER BY symbol ASC", user=session['user_id'])

        owned = {}

        # insert data into owned companies dictionary
        for company in rows:

            # get data for owned company using IEX api
            quote = lookup(company['symbol'])

            # set dictionary index to company symbol and store a dictionary of data for company at index
            owned[company['symbol']] = {'name': quote['name']}

        return render_template("sell.html", owned=rows, data=owned)

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
