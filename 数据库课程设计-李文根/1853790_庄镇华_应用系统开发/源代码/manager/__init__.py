from flask import render_template, flash, request, redirect, url_for, session, jsonify, Blueprint
from functools import wraps
from tools import MySQLTools

manager = Blueprint('manager', __name__)
# 初始化数据库
util = MySQLTools('localhost', 'root', 'cfpl', 'sharebike', 'utf8')

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

# 主页
@manager.route('/index', methods = ['GET', 'POST'])
@login_required
def index():
    user_cnt, bike_cnt, order_cnt, news_cnt = util.dataStatic()
    user_list, bike_list, order_list, news_list = util.timeDataStatic()
    print(bike_cnt, bike_list)
    return render_template("/manager/index.html",
                           user_cnt = user_cnt, bike_cnt = bike_cnt,
                           order_cnt = order_cnt, news_cnt = news_cnt,
                           user_list = user_list, bike_list = bike_list,
                           order_list = order_list, news_list = news_list)