from flask import render_template, flash, request, redirect, url_for, session, jsonify, Blueprint
from functools import wraps
from tools import MySQLTools

manager_news = Blueprint('manager_news', __name__)
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

@manager_news.route('/main', methods = ['GET', 'POST'])
@login_required
def main():
    new = util.news_All()
    return render_template("/manager/news/news.html", articles = new)

@manager_news.route("/detail/<news_id>", methods = ['GET', 'POST'])
@login_required
def news_detail(news_id):
    util.addNewsViews(news_id)
    new = util.news(news_id)
    return render_template("/manager/news/news_detail.html", article = new[0])

@manager_news.route("/update/<news_id>", methods = ['GET', 'POST'])
@login_required
def news_update(news_id):
    new = util.news(news_id)
    if request.method == "POST":
        util.subNewsViews(news_id)
        form = request.form
        title = form.get('title')
        body = form.get('body')
        util.updateNews(news_id, title, body)
        return redirect(url_for('manager_news.news_detail', news_id = news_id))
    return render_template("/manager/news/news_update.html", article = new[0])

@manager_news.route("/delete/<news_id>", methods = ['GET', 'POST'])
@login_required
def news_delete(news_id):
    util.deleteNews(news_id)
    return redirect(url_for('manager_news.main'))

@manager_news.route("/news_create", methods = ['GET', 'POST'])
@login_required
def news_create():
    if request.method == "POST":
        form = request.form
        title = form.get('title')
        body = form.get('body')
        util.addNews(title, body, session['user'])
        return redirect(url_for('manager_news.main'))
    return render_template("/manager/news/news_create.html")
