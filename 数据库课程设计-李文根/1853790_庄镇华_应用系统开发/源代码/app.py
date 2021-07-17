from flask import Flask, render_template, flash, request, redirect, url_for, session
from tools import MySQLTools
from functools import wraps

from manager.bike import manager_bike as manager_bike_blueprint
from manager.user import manager_user as manager_user_blueprint
from manager.hot import manager_hot as manager_hot_blueprint
from manager.news import manager_news as manager_news_blueprint
from manager import manager as manager_blueprint

from user.bike import user_bike as user_bike_blueprint
from user.news import user_news as user_news_blueprint
from user.hot import user_hot as user_hot_blueprint
from user import user as user_blueprint

# 初始化数据库
util = MySQLTools('localhost', 'root', 'cfpl', 'sharebike', 'utf8')
# 初始化flask框架
app = Flask(__name__)
app.secret_key = "jJInfewp(8efkd*9&jfkl"

bike_id = ""
user_id = ""

# 判断用户是否登录成功
def login_required(func):
    @wraps(func)
    def wrapper(*args, **kwargs):
        if session.get('user', None):
            return func(*args, **kwargs)
        else:
            flash("请先登录", 'danger')
            return redirect(url_for('login'))
    return wrapper

@app.template_filter('slice')
def sub(str, start, end):
    return str[start : end]

@app.before_first_request
def create_db():
    # 创建数据库、表单，插入初始数据
    util.create_all()
    return

# 登录界面路由
@app.route('/', methods = ['GET', 'POST'])
def login():
    if request.method == 'POST':
        form = request.form
        userid = form.get('user_id')
        username = form.get('user_name')
        userpass = form.get('user_pass')
        identity = form.get('identity')
        if not userid:
            flash("请输入学号/工号", 'danger')
            return render_template("login.html", user_name = username, user_pass = userpass)
        if not username:
            flash("请输入姓名", 'danger')
            return render_template("login.html", user_id = userid, user_pass = userpass)
        if not userpass:
            flash("请输入密码", 'danger')
            return render_template("login.html", user_id = userid, user_name = username)
        if identity != '管理员':
            if (util.checkUserPassword(userid, userpass)):
                session['user'] = userid
                return redirect(url_for('user.index'))
            else:
                flash("用户名或密码错误", 'danger')
                return render_template("login.html", user_id=userid, user_name=username)
        else:
            if (util.checkManagerPassword(userid, userpass)):
                session['user'] = userid
                return redirect(url_for('manager.index'))
            else:
                flash("用户名或密码错误", 'danger')
                return render_template("login.html", user_id=userid, user_name=username)
    return render_template('login.html')

# 注册界面路由
@app.route('/register', methods = ['GET', 'POST'])
def register():
    if request.method == "POST":
        form = request.form
        userid = form.get('user_id')
        username = form.get('user_name')
        useremail = form.get('user_email')
        userpass = form.get('user_pass')
        identity = form.get('identity')
        if not userid:
            flash("请输入学号", 'danger')
            return render_template("register.html")
        if not username:
            flash("请输入姓名", 'danger')
            return render_template("register.html")
        if not useremail:
            flash("请输入邮箱", 'danger')
            return render_template("register.html")
        if not userpass:
            flash("请输入密码", 'danger')
            return render_template("register.html")
        else:
            if identity != '管理员':
                state = util.registerUserAdmin(userid, username, useremail, userpass)
                if (state == "existed"):
                    flash("用户名重复", 'danger')
                    return render_template("register.html")
                flash("注册成功", 'danger')
                return redirect(url_for('login'))
            else:
                state = util.registerManagerAdmin(userid, username, useremail, userpass)
                if (state == "existed"):
                    flash("用户名重复", 'danger')
                    return render_template("register.html")
                flash("注册成功", 'danger')
                return redirect(url_for('login'))
    return render_template("register.html")

@app.route('/logout', methods = ['GET', 'POST'])
@login_required
def logout():
    session.pop('user')
    return redirect(url_for('login'))

# 错误界面（异常处理）
@app.errorhandler(404)
@login_required
def not_found(error):
    return render_template("404.html")

@app.route('/test', methods = ['GET', 'POST'])
def test():
    return render_template("test.html")

# 注册蓝图
app.register_blueprint(manager_bike_blueprint, url_prefix = '/manager/bike')
app.register_blueprint(manager_user_blueprint, url_prefix = '/manager/user')
app.register_blueprint(manager_hot_blueprint, url_prefix = '/manager/hot')
app.register_blueprint(manager_news_blueprint, url_prefix = '/manager/news')
app.register_blueprint(manager_blueprint, url_prefix = '/manager')

app.register_blueprint(user_bike_blueprint, url_prefix = '/user/bike')
app.register_blueprint(user_news_blueprint, url_prefix = '/user/news')
app.register_blueprint(user_hot_blueprint, url_prefix = '/user/hot')
app.register_blueprint(user_blueprint, url_prefix = '/user')

if __name__ == '__main__':
    app.run()
