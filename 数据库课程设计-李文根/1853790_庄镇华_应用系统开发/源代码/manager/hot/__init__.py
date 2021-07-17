from flask import render_template, flash, request, redirect, url_for, session, jsonify, Blueprint
from functools import wraps
from tools import MySQLTools

manager_hot = Blueprint('manager_hot', __name__)
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

@manager_hot.route('/main', methods = ['GET', 'POST'])
@login_required
def main():
    points = util.getPoints()
    if request.method == "POST":
        form = request.form
        bike_id = form.get('mybike0')
        bike_brand = form.get('mybike1')
        bike_lon = form.get('mybike2')
        bike_lat = form.get('mybike3')
        if bike_id and bike_brand and bike_lon and bike_lat:
            util.addBike(bike_id, bike_brand, bike_lon, bike_lat)
    return render_template("/manager/hot/hot.html", points = points)
