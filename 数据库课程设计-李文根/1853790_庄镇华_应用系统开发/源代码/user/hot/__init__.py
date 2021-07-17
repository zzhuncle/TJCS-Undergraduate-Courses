from flask import render_template, flash, request, redirect, url_for, session, jsonify, Blueprint
from functools import wraps
from tools import MySQLTools

user_hot = Blueprint('user_hot', __name__)
# 初始化数据库
util = MySQLTools('localhost', 'root', 'cfpl', 'sharebike', 'utf8')

# 判断用户是否登录成功
def login_required(func):
    @wraps(func)  # 继承原函数，调用时不改变函数原含义
    def wrapper(*args, **kwargs):
        if session.get('user', None):
            return func(*args, **kwargs)
        else:
            flash("请先登录", 'danger')
            return redirect(url_for('login'))
    return wrapper

@user_hot.route('/main', methods = ['GET', 'POST'])
@login_required
def main():
    points = util.getPoints()
    return render_template("/user/hot/hot.html", points = points)
