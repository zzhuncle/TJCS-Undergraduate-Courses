from flask import render_template, flash, request, redirect, url_for, session, jsonify, Blueprint
from functools import wraps
from tools import MySQLTools

manager_user = Blueprint('manager_user', __name__)
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

@manager_user.route('/add', methods = ['GET', 'POST'])
@login_required
def user_add():
    if request.method == "POST":
        form = request.form
        user_id = form.get('mycontent1')
        user_name = form.get('mycontent2')
        user_email = form.get('mycontent3')
        user_pass = form.get('mycontent4')
        user_type = form.get('mycontent5')
        if user_id and user_name and user_email and user_pass:
            if user_type == '0':
                util.addUser(user_id, user_name, user_email, user_pass)
            else:
                util.addManager(user_id, user_name, user_email, user_pass)
    return render_template("/manager/user/user_add.html")

@manager_user.route('/delete', methods = ['GET', 'POST'])
@login_required
def user_delete():
    if request.method == "POST":
        form = request.form
        user_id = form.get('mycontent1')
        user_type = form.get('mycontent2')
        if user_id and user_type:
            if user_type == '0':
                util.deleteUser(user_id)
            else:
                util.deleteManager(user_id)
    return render_template("/manager/user/user_delete.html")

@manager_user.route('/query', methods = ['GET', 'POST'])
@login_required
def user_query():
    if request.method == "POST":
        form = request.form
        userid = form.get('mycontent')
        if userid:
            global user_id
            user_id = userid
    return render_template("/manager/user/user_query.html")

@manager_user.route('/userquerydata', methods=['POST', 'GET'])
def user_query_data():
    global user_id
    data = util.user_bikeAll(user_id)
    if request.method == 'GET':
        info = request.values
        limit = info.get('limit', 10)
        offset = info.get('offset', 0)
    return jsonify({'total': len(data), 'rows': data[int(offset):(int(offset) + int(limit))]})

@manager_user.route('/userdata', methods=['POST', 'GET'])
def user_data():
    data = util.allUserInfo()
    if request.method == 'GET':
        info = request.values
        limit = info.get('limit', 10)
        offset = info.get('offset', 0)
    return jsonify({'total': len(data), 'rows': data[int(offset):(int(offset) + int(limit))]})

@manager_user.route('/managerdata', methods=['POST', 'GET'])
def manager_data():
    data = util.allManagerInfo()
    if request.method == 'GET':
        info = request.values
        limit = info.get('limit', 10)
        offset = info.get('offset', 0)
    return jsonify({'total': len(data), 'rows': data[int(offset):(int(offset) + int(limit))]})
