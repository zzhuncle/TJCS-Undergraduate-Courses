import pymysql

# 将pymysql的查询结果转化为json格式
def sql_fetch_json(cursor: pymysql.cursors.Cursor):
    keys = []
    for column in cursor.description:
        keys.append(column[0])
    key_number = len(keys)

    json_data = []
    for row in cursor.fetchall():
        item = dict()
        for q in range(key_number):
            item[keys[q]] = row[q]
        json_data.append(item)
    return json_data

class MySQLTools():
    # 初始化
    def __init__(self, host, user, password, db, charset):
        self.host = host
        self.user = user
        self.password = password
        self.db = db
        self.charset = charset
        self.conn = pymysql.connect(host = host, user = user, password = password, db = db, charset = charset)
        self.cur = self.conn.cursor()

    # 创建数据库、表单，插入初始数据
    def create_all(self):
        with open('bike.sql', 'r', encoding = 'utf-8') as init_file:
            text = init_file.read()
            lines = text.split('----')
            for line in lines:
                self.cur.execute(line)
                if 'insert' in line or 'update' in line or 'delete' in line:
                    self.conn.commit()

    # 检查用户合法性
    def checkUserPassword(self, user_id, user_pass):
        sqlstr = f"select * from users where user_id = '{user_id}' and user_pass = '{user_pass}' limit 1;"
        self.cur.execute(sqlstr)
        result = self.cur.fetchone()
        return result != None

    # 检查管理员合法性
    def checkManagerPassword(self, manager_id, manager_pass):
        sqlstr = f"select * from manager where work_id = '{manager_id}' and manager_pass = '{manager_pass}' limit 1;"
        self.cur.execute(sqlstr)
        result = self.cur.fetchone()
        return result != None

    # 注册用户
    def registerUserAdmin(self, userid, username, useremail, userpass):
        sqlstr = "select user_pass from users where user_name = '" + username + "';"
        self.cur.execute(sqlstr)
        result = self.cur.fetchall()
        if result:
            return "existed"
        sqlstr = "insert into users(user_id, user_name, user_email, user_pass) values('" + userid + "','" + username + "','" \
                                    + useremail + "','" + userpass + "');"
        self.cur.execute(sqlstr)
        # 涉及写操作注意提交
        self.conn.commit()

    # 注册管理员
    def registerManagerAdmin(self, userid, username, useremail, userpass):
        sqlstr = "select manager_pass from manager where manager_name = '" + username + "';"
        self.cur.execute(sqlstr)
        result = self.cur.fetchall()
        if result:
            return "existed"
        sqlstr = "insert into manager(work_id, manager_name, manager_email, manager_pass) values('" + userid + "','" + username + "','" \
                                    + useremail + "','" + userpass + "');"
        self.cur.execute(sqlstr)
        # 涉及写操作注意提交
        self.conn.commit()

    # 数据统计
    def dataStatic(self):
        sqlstr = "select count(*) as cnt from users;"
        self.cur.execute(sqlstr)
        users = self.cur.fetchone()
        sqlstr = "select count(*) as cnt from bike;"
        self.cur.execute(sqlstr)
        bike = self.cur.fetchone()
        sqlstr = "select count(*) as cnt from rent;"
        self.cur.execute(sqlstr)
        rent = self.cur.fetchone()
        sqlstr = "select count(*) as cnt from news;"
        self.cur.execute(sqlstr)
        news = self.cur.fetchone()
        return users[0], bike[0], rent[0], news[0]

    # 单车品牌占比统计
    def numStatic(self):
        sqlstr = "select count(*) as cnt from bike where bike_brand = 'mobike';"
        self.cur.execute(sqlstr)
        n1 = self.cur.fetchone()[0]
        sqlstr = "select count(*) as cnt from bike where bike_brand = 'OfO';"
        self.cur.execute(sqlstr)
        n2 = self.cur.fetchone()[0]
        sqlstr = "select count(*) as cnt from bike where bike_brand = '哈啰';"
        self.cur.execute(sqlstr)
        n3 = self.cur.fetchone()[0]
        sqlstr = "select count(*) as cnt from bike where bike_brand = 'ubike';"
        self.cur.execute(sqlstr)
        n4 = self.cur.fetchone()[0]
        n = n1 + n2 + n3 + n4
        return int(n1 / n * 100), int(n2 / n * 100), int(n3 / n * 100), int(n4 / n * 100)

    # 时间序列统计
    def timeDataStatic(self):
        user_list, bike_list, rent_list, news_list = [], [], [], []
        years = [20, 21, 21, 21, 21, 21, 21]
        months = [12, 1, 2, 3, 4, 5, 6, 7]
        for i in range(len(years)):
            sqlstr = f"select count(*) as cnt from users where user_time >= '20{years[i]}-{months[i]:02d}-01 00:00:00' and user_time < '20{years[i]}-{months[i + 1]:02d}-01 00:00:00'"
            self.cur.execute(sqlstr)
            user_list.append(self.cur.fetchone()[0])
        for i in range(len(years)):
            sqlstr = f"select count(*) as cnt from bike where bike_time >= '20{years[i]}-{months[i]:02d}-01 00:00:00' and bike_time < '20{years[i]}-{months[i + 1]:02d}-01 00:00:00'"
            self.cur.execute(sqlstr)
            bike_list.append(self.cur.fetchone()[0])
        for i in range(len(years)):
            sqlstr = f"select count(*) as cnt from rent where rent_time >= '20{years[i]}-{months[i]:02d}-01 00:00:00' and rent_time < '20{years[i]}-{months[i + 1]:02d}-01 00:00:00'"
            self.cur.execute(sqlstr)
            rent_list.append(self.cur.fetchone()[0])
        for i in range(len(years)):
            sqlstr = f"select count(*) as cnt from news where news_created >= '20{years[i]}-{months[i]:02d}-01 00:00:00' and news_created < '20{years[i]}-{months[i + 1]:02d}-01 00:00:00'"
            self.cur.execute(sqlstr)
            news_list.append(self.cur.fetchone()[0])
        return user_list, bike_list, rent_list, news_list

    # 删除单车
    def deleteBike(self, bike_id):
        sqlstr = f"delete from bike where bike_id = '{bike_id}';"
        self.cur.execute(sqlstr)
        self.conn.commit()
        return

    # 删除用户
    def deleteUser(self, user_id):
        sqlstr = f"delete from users where user_id = '{user_id}';"
        self.cur.execute(sqlstr)
        self.conn.commit()
        return

    # 删除超级用户
    def deleteManager(self, work_id):
        sqlstr = f"delete from manager where work_id = '{work_id}';"
        self.cur.execute(sqlstr)
        self.conn.commit()
        return

    # 增添单车
    def addBike(self, bike_id, bike_brand, bike_lon, bike_lat):
        sqlstr = f"insert ignore into bike(bike_id, bike_brand, bike_cnt, bike_lon, bike_lat, bike_status) values('{bike_id}','{bike_brand}', 0, {bike_lon}, {bike_lat}, 'false');"
        self.cur.execute(sqlstr)
        self.conn.commit()
        return

    # 添加用户
    def addUser(self, user_id, user_name, user_email, user_pass):
        sqlstr = f"insert ignore into users(user_id, user_name, user_email, user_pass) values('{user_id}','{user_name}', '{user_email}', '{user_pass}');"
        self.cur.execute(sqlstr)
        self.conn.commit()
        return

    # 添加超级用户
    def addManager(self, work_id, manager_name, manager_email, manager_pass):
        sqlstr = f"insert ignore into manager(work_id, manager_name, manager_email, manager_pass) values('{work_id}','{manager_name}', '{manager_email}', '{manager_pass}');"
        self.cur.execute(sqlstr)
        self.conn.commit()
        return

    # 所有单车信息
    def allBikeInfo(self):
        sqlstr = "select * from bike;"
        self.cur.execute(sqlstr)
        return sql_fetch_json(self.cur)

    # 所有租借信息
    def allRentInfo(self):
        sqlstr = "select * from rent;"
        self.cur.execute(sqlstr)
        return sql_fetch_json(self.cur)

    # 所有归还信息
    def allReturnInfo(self):
        sqlstr = "select * from retur;"
        self.cur.execute(sqlstr)
        return sql_fetch_json(self.cur)

    # 个人租借信息
    def personRentInfo(self, user_id):
        sqlstr = f"select * from rent where user_id = '{user_id}';"
        self.cur.execute(sqlstr)
        return sql_fetch_json(self.cur)

    # 个人归还信息
    def personReturnInfo(self, user_id):
        sqlstr = f"select * from retur where user_id = '{user_id}';"
        self.cur.execute(sqlstr)
        return sql_fetch_json(self.cur)

    # 所有归还信息
    def allUserInfo(self):
        sqlstr = "select * from users;"
        self.cur.execute(sqlstr)
        return sql_fetch_json(self.cur)

    # 所有归还信息
    def allManagerInfo(self):
        sqlstr = "select * from manager;"
        self.cur.execute(sqlstr)
        return sql_fetch_json(self.cur)

    # 租借单车
    def rentBike(self, user_id, bike_id):
        sqlstr = f"select bike_status from bike where bike_id = '{bike_id}';"
        self.cur.execute(sqlstr)
        status = str(self.cur.fetchone()[0])
        if status == 'false':
            sqlstr = f"insert ignore into rent(user_id, bike_id) values('{user_id}','{bike_id}');"
            self.cur.execute(sqlstr)
            self.conn.commit()
            sqlstr = f"update bike set bike_status = 'true' where bike_id = '{bike_id}';"
            self.cur.execute(sqlstr)
        return

    # 归还单车
    def returnBike(self, user_id, bike_id):
        # 检查单车状态
        sqlstr = f"select bike_status from bike where bike_id = '{bike_id}';"
        self.cur.execute(sqlstr)
        status = str(self.cur.fetchone()[0])
        if status == 'true':
            sqlstr = f"select _id_ from rent where bike_id = '{bike_id}' order by _id_ desc;"
            self.cur.execute(sqlstr)
            id_list = self.cur.fetchall()
            _id_ = id_list[0][0]
            sqlstr = f"insert ignore into retur(_id_, user_id, bike_id) values('{_id_}', '{user_id}','{bike_id}');"
            self.cur.execute(sqlstr)
            self.conn.commit()
            # 更新单车状态
            sqlstr = f"update bike set bike_status = 'false' where bike_id = '{bike_id}';"
            self.cur.execute(sqlstr)
            # 单车使用次数加1
            sqlstr = f"update bike set bike_cnt = bike_cnt + 1 where bike_id = '{bike_id}';"
            self.cur.execute(sqlstr)
        return

    # 单车查询
    def bike_userAll(self, bike_id):
        sqlstr = f"select rent.user_id, rent.bike_id, rent.rent_time, retur.return_time " \
                 f"from rent left outer join retur " \
                 f"on rent._id_ = retur._id_ and rent.user_id = retur.user_id and rent.bike_id = retur.bike_id " \
                 f"where rent.bike_id = '{bike_id}' " \
                 f"order by rent.rent_time asc;"
        self.cur.execute(sqlstr)
        return sql_fetch_json(self.cur)

    # 用户查询
    def user_bikeAll(self, user_id):
        sqlstr = f"select rent.user_id, rent.bike_id, rent.rent_time, retur.return_time " \
                 f"from rent left outer join retur " \
                 f"on rent._id_ = retur._id_ and rent.user_id = retur.user_id and rent.bike_id = retur.bike_id " \
                 f"where rent.user_id = '{user_id}' " \
                 f"order by rent.rent_time asc;"
        self.cur.execute(sqlstr)
        return sql_fetch_json(self.cur)

    # 热度图查询
    def getPoints(self):
        sqlstr = "select bike_lon, bike_lat from bike;"
        self.cur.execute(sqlstr)
        keys = ["lng", "lat"]
        json_data = []
        for row in self.cur.fetchall():
            item = dict()
            for q in range(2):
                item[keys[q]] = row[q]
            item["count"] = 100
            json_data.append(item)
        return json_data

    def news_All(self):
        sqlstr = f"select * from news;"
        self.cur.execute(sqlstr)
        return sql_fetch_json(self.cur)

    def news(self, news_id):
        sqlstr = f"select * from news where news_id = {news_id};"
        self.cur.execute(sqlstr)
        return sql_fetch_json(self.cur)

    def addNews(self, title, body, user):
        sqlstr = f"insert ignore into news (news_title, news_body, news_author, news_created, news_updated, total_views) values('{title}', '{body}', '{user}', current_timestamp, current_timestamp, 0);"
        self.cur.execute(sqlstr)
        self.conn.commit()
        return

    def deleteNews(self, news_id):
        sqlstr = f"delete from news where news_id = '{news_id}'"
        self.cur.execute(sqlstr)
        self.conn.commit()
        return

    def addNewsViews(self, news_id):
        sqlstr = f"update news set total_views = total_views + 1 where news_id = '{news_id}';"
        self.cur.execute(sqlstr)
        return

    def subNewsViews(self, news_id):
        sqlstr = f"update news set total_views = total_views - 1 where news_id = '{news_id}';"
        self.cur.execute(sqlstr)
        return

    def updateNews(self, news_id, title, body):
        sqlstr = f"update news set news_title = '{title}', news_body = '{body}', news_updated = current_timestamp where news_id = '{news_id}';"
        self.cur.execute(sqlstr)
        return
