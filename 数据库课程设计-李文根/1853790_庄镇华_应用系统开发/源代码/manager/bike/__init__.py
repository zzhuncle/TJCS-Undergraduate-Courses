from flask import render_template, flash, request, redirect, url_for, session, jsonify, Blueprint
from functools import wraps
from tools import MySQLTools

manager_bike = Blueprint('manager_bike', __name__)
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

@manager_bike.route('/add', methods = ['GET', 'POST'])
@login_required
def bike_add():
    if request.method == "POST":
        form = request.form
        bike_id = form.get('mybike0')
        bike_brand = form.get('mybike1')
        bike_lon = form.get('mybike2')
        bike_lat = form.get('mybike3')
        if bike_id and bike_brand and bike_lon and bike_lat:
            util.addBike(bike_id, bike_brand, bike_lon, bike_lat)
    return render_template("/manager/bike/bike_add.html")

@manager_bike.route('/delete', methods = ['GET', 'POST'])
@login_required
def bike_delete():
    if request.method == "POST":
        form = request.form
        bike_id = form.get('mybike')
        if bike_id:
            util.deleteBike(bike_id)
    return render_template("/manager/bike/bike_delete.html")

@manager_bike.route('/rent', methods = ['GET', 'POST'])
@login_required
def bike_rent():
    if request.method == "POST":
        form = request.form
        bike_id = form.get('mybike')
        if bike_id:
            util.rentBike(session['user'], bike_id)
    return render_template("/manager/bike/bike_rent.html")

@manager_bike.route('/return', methods = ['GET', 'POST'])
@login_required
def bike_return():
    if request.method == "POST":
        form = request.form
        bike_id = form.get('mybike')
        if bike_id:
            util.returnBike(session['user'], bike_id)
    return render_template("/manager/bike/bike_return.html")

@manager_bike.route('/query', methods = ['GET', 'POST'])
@login_required
def bike_query():
    if request.method == "POST":
        form = request.form
        bikeid = form.get('mybike')
        if bikeid:
            global bike_id
            bike_id = bikeid
    return render_template("/manager/bike/bike_query.html")

@manager_bike.route('/bikedata', methods=['POST', 'GET'])
def bike_data():
    data = util.allBikeInfo()
    if request.method == 'GET':
        info = request.values
        limit = info.get('limit', 10)
        offset = info.get('offset', 0)
    return jsonify({'total': len(data), 'rows': data[int(offset) : (int(offset) + int(limit))]})

@manager_bike.route('/rentdata', methods=['POST', 'GET'])
def rent_data():
    data = util.allRentInfo()
    if request.method == 'GET':
        info = request.values
        limit = info.get('limit', 10)
        offset = info.get('offset', 0)
    return jsonify({'total': len(data), 'rows': data[int(offset):(int(offset) + int(limit))]})

@manager_bike.route('/returndata', methods=['POST', 'GET'])
def return_data():
    data = util.allReturnInfo()
    if request.method == 'GET':
        info = request.values
        limit = info.get('limit', 10)
        offset = info.get('offset', 0)
    return jsonify({'total': len(data), 'rows': data[int(offset):(int(offset) + int(limit))]})

@manager_bike.route('/bikequerydata', methods=['POST', 'GET'])
def bike_query_data():
    global bike_id
    data = util.bike_userAll(bike_id)
    if request.method == 'GET':
        info = request.values
        limit = info.get('limit', 10)
        offset = info.get('offset', 0)
    return jsonify({'total': len(data), 'rows': data[int(offset):(int(offset) + int(limit))]})

