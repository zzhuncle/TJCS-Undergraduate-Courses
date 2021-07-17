-- 为了自动读取本文件并初始化, 需要在每条语句后加 ----
-- 但注意最后一句不要加 ----

-- 建立数据库
drop database if exists sharebike; ----
create database sharebike; ----
use sharebike; ----

-- 建立表单
drop table if exists users; ----
create table users(
    user_id char(8) primary key,
    user_name char(20) not null unique,
    user_email char(20) not null,
    user_pass char(20) not null,
    user_time timestamp not null default current_timestamp
); ----
drop table if exists manager; ----
create table manager(
    work_id char(8) primary key,
    manager_name char(20) not null,
    manager_email char(20) not null,
    manager_pass char(20) not null,
    manager_time timestamp not null default current_timestamp
); ----
drop table if exists bike; ----
create table bike(
    bike_id char(10) primary key,
    bike_brand char(20) not null,
    bike_cnt int not null default 1,
    bike_status enum('true', 'false') default 'false',
    bike_lon float(15,10) not null default 121.510025,
    bike_lat float(15,10) not null default 31.289031,
    bike_time timestamp not null default current_timestamp
); ----
drop table if exists rent; ----
create table rent(
    _id_ bigint(20) primary key auto_increment,
    user_id char(8) not null,
    bike_id char(10) not null,
    rent_time timestamp not null default current_timestamp,
    constraint rent_fk0 foreign key(user_id) references users(user_id) on delete cascade on update cascade,
    constraint rent_fk1 foreign key(bike_id) references bike(bike_id)  on delete cascade on update cascade
); ----
drop table if exists retur; ----
create table retur(
    _id_ bigint(20) primary key auto_increment,
    user_id char(8) not null,
    bike_id char(10) not null,
    return_time timestamp not null default current_timestamp,
    constraint return_fk0 foreign key(user_id) references users(user_id) on delete cascade on update cascade,
    constraint return_fk1 foreign key(bike_id) references bike(bike_id) on delete cascade on update cascade
); ----
drop table if exists news; ----
create table news(
    news_id bigint(20) primary key auto_increment,
    news_title char(128) not null,
    news_body mediumtext not null,
    news_author char(20) not null,
    news_created timestamp not null default current_timestamp,
    news_updated timestamp not null default current_timestamp,
    total_views bigint(20) not null default 0,
    constraint news_fk0 foreign key(news_author) references users(user_name) on delete cascade on update cascade
); ----

-- 插入数据
-- insert ignore 若已存在相同主键，忽略插入的新数据
insert ignore into users(user_id, user_name, user_email, user_pass, user_time)
    values('123', '123', '123', '123', current_timestamp); ----
insert ignore into users(user_id, user_name, user_email, user_pass, user_time)
    values('0000001', 'test', 'test@tongji.edu.cn', 'test', '2020-01-01 00:00:00'); ----
insert ignore into users(user_id, user_name, user_email, user_pass, user_time)
    values('1853790', 'zzh', 'zzh1814@tongji.edu.cn', '123', current_timestamp); ----
insert ignore into users(user_id, user_name, user_email, user_pass, user_time)
    values('1850000', 'test0','zzy99@126.com','123', '2021-03-22 12:00:00'); ----
insert ignore into users(user_id, user_name, user_email, user_pass, user_time)
    values('1850001', 'test1','zzy99@126.com','123', '2021-03-20 12:00:00'); ----
insert ignore into users(user_id, user_name, user_email, user_pass, user_time)
    values('1850002', 'test2','zzy99@126.com','123', '2021-02-22 12:00:00'); ----
insert ignore into users(user_id, user_name, user_email, user_pass, user_time)
    values('1850003', 'test3','zzy99@126.com','123', '2021-02-22 12:00:00'); ----
insert ignore into users(user_id, user_name, user_email, user_pass, user_time)
    values('1850004', 'test4','zzy99@126.com','123', '2021-01-22 12:00:00'); ----
insert ignore into users(user_id, user_name, user_email, user_pass, user_time)
    values('1850005', 'test5','zzy99@126.com','123', '2021-01-22 12:00:00'); ----
insert ignore into users(user_id, user_name, user_email, user_pass, user_time)
    values('1850006', 'test6','zzy99@126.com','123', '2021-01-22 12:00:00'); ----
insert ignore into users(user_id, user_name, user_email, user_pass, user_time)
    values('1850007', 'test7','zzy99@126.com','123', '2021-01-22 12:00:00'); ----
insert ignore into users(user_id, user_name, user_email, user_pass, user_time)
    values('1850008', 'test8','zzy99@126.com','123', '2020-12-22 12:00:00'); ----
insert ignore into users(user_id, user_name, user_email, user_pass, user_time)
    values('1850009', 'test9','zzy99@126.com','123', '2020-12-22 12:00:00'); ----
insert ignore into users(user_id, user_name, user_email, user_pass, user_time)
    values('1850010', 'test10','zzy99@126.com','123', '2020-12-22 12:00:00'); ----
insert ignore into users(user_id, user_name, user_email, user_pass, user_time)
    values('1850011', 'test11','zzy99@126.com','123', '2020-12-22 12:00:00'); ----
insert ignore into users(user_id, user_name, user_email, user_pass, user_time)
    values('1850012', 'test12','zzy99@126.com','123', '2020-12-22 12:00:00'); ----
insert ignore into users(user_id, user_name, user_email, user_pass, user_time)
    values('1850013', 'test13','zzy99@126.com','123', '2020-12-22 12:00:00'); ----
insert ignore into users(user_id, user_name, user_email, user_pass, user_time)
    values('1850014', 'test14','zzy99@126.com','123', '2020-12-22 12:00:00'); ----
insert ignore into users(user_id, user_name, user_email, user_pass, user_time)
    values('1850015', 'test15','zzy99@126.com','123', '2020-11-22 12:00:00'); ----
insert ignore into users(user_id, user_name, user_email, user_pass, user_time)
    values('1850016', 'test16','zzy99@126.com','123', '2020-11-22 12:00:00'); ----

insert ignore into manager(work_id, manager_name, manager_email, manager_pass, manager_time)
    values('123', '123', '123', '123', current_timestamp); ----
insert ignore into manager(work_id, manager_name, manager_email, manager_pass, manager_time)
    values('0000002', 'super', 'super@tongji.edu.cn', 'super', '2020-04-04 00:00:00'); ----
insert ignore into manager(work_id, manager_name, manager_email, manager_pass, manager_time)
    values('0000001', 'root', 'bikes@tongji.edu.cn', 'bikes', '2020-01-01 00:00:00'); ----

insert ignore into bike (bike_id, bike_brand, bike_cnt, bike_status, bike_time, bike_lon, bike_lat)
    values('000001', 'mobike', 0, 'false', current_timestamp, 121.218333,31.292904); ----
insert ignore into bike (bike_id, bike_brand, bike_cnt, bike_status, bike_time, bike_lon, bike_lat)
    values('000002', 'OfO', 0, 'false', current_timestamp, 121.218333,31.292904); ----
insert ignore into bike (bike_id, bike_brand, bike_cnt, bike_status, bike_time, bike_lon, bike_lat)
    values('000003', '哈啰', 10, 'false', '2021-03-22 12:00:00', 121.218854,31.292811); ----
insert ignore into bike (bike_id, bike_brand, bike_cnt, bike_status, bike_time, bike_lon, bike_lat)
    values('000004', 'ubike', 12, 'false', '2021-03-22 12:00:00', 121.221118,31.293382); ----
insert ignore into bike (bike_id, bike_brand, bike_cnt, bike_status, bike_time, bike_lon, bike_lat)
    values('000005', 'mobike', 8, 'false', '2021-03-22 12:00:00', 121.221567,31.293552); ----
insert ignore into bike (bike_id, bike_brand, bike_cnt, bike_status, bike_time, bike_lon, bike_lat)
    values('000006', 'mobike', 12, 'false', '2021-03-22 12:00:00', 121.219034,31.293598); ----
insert ignore into bike (bike_id, bike_brand, bike_cnt, bike_status, bike_time, bike_lon, bike_lat)
    values('000007', 'mobike', 12, 'false', '2021-03-22 12:00:00', 121.219375,31.293505); ----
insert ignore into bike (bike_id, bike_brand, bike_cnt, bike_status, bike_time, bike_lon, bike_lat)
    values('000008', 'OfO', 21, 'false', '2021-03-22 12:00:00', 121.220076,31.291685); ----
insert ignore into bike (bike_id, bike_brand, bike_cnt, bike_status, bike_time, bike_lon, bike_lat)
    values('000009', 'OfO', 21, 'false', '2021-03-22 12:00:00', 121.221118,31.292857); ----
insert ignore into bike (bike_id, bike_brand, bike_cnt, bike_status, bike_time, bike_lon, bike_lat)
    values('000010', '哈啰', 13, 'false', '2021-03-22 12:00:00', 121.225034,31.29187); ----
insert ignore into bike (bike_id, bike_brand, bike_cnt, bike_status, bike_time, bike_lon, bike_lat)
    values('000011', '哈啰', 17, 'false', '2021-03-22 12:00:00', 121.225034,31.29187); ----
insert ignore into bike (bike_id, bike_brand, bike_cnt, bike_status, bike_time, bike_lon, bike_lat)
    values('000012', 'ubike', 12, 'false','2021-02-22 12:00:00', 121.224675,31.289741); ----
insert ignore into bike (bike_id, bike_brand, bike_cnt, bike_status, bike_time, bike_lon, bike_lat)
    values('000013', 'mobike', 9, 'false', '2021-02-22 12:00:00', 121.217417,31.295017); ----
insert ignore into bike (bike_id, bike_brand, bike_cnt, bike_status, bike_time, bike_lon, bike_lat)
    values('000014', 'ubike', 10, 'false', '2021-02-22 12:00:00', 121.217363,31.294678); ----
insert ignore into bike (bike_id, bike_brand, bike_cnt, bike_status, bike_time, bike_lon, bike_lat)
    values('000015', '哈啰', 40, 'false', '2021-02-22 12:00:00', 121.22189,31.290944); ----
insert ignore into bike (bike_id, bike_brand, bike_cnt, bike_status, bike_time, bike_lon, bike_lat)
    values('000016', '哈啰', 36, 'false', '2021-01-22 12:00:00', 121.224585,31.293274); ----
insert ignore into bike (bike_id, bike_brand, bike_cnt, bike_status, bike_time, bike_lon, bike_lat)
    values('000017', 'mobike', 30, 'false', '2021-01-22 12:00:00', 121.218207,31.292302); ----
insert ignore into bike (bike_id, bike_brand, bike_cnt, bike_status, bike_time, bike_lon, bike_lat)
    values('000018', 'mobike', 27, 'false', '2021-01-22 12:00:00', 121.221603,31.295033); ----
insert ignore into bike (bike_id, bike_brand, bike_cnt, bike_status, bike_time, bike_lon, bike_lat)
    values('000019', 'mobike', 21, 'false', '2021-01-22 12:00:00', 121.223166,31.291376); ----
insert ignore into bike (bike_id, bike_brand, bike_cnt, bike_status, bike_time, bike_lon, bike_lat)
    values('000020', 'ubike', 7, 'false', '2021-01-22 12:00:00', 121.219016,31.28988); ----
insert ignore into bike (bike_id, bike_brand, bike_cnt, bike_status, bike_time, bike_lon, bike_lat)
    values('000021', 'OfO', 8, 'false', '2020-11-22 12:00:00', 121.217506,31.293073); ----

insert ignore into rent (user_id, bike_id, rent_time)
    values('1853790', '000001', current_timestamp); ----
insert ignore into rent (user_id, bike_id, rent_time)
    values('1850002', '000002', '2021-03-22 12:00:00'); ----
insert ignore into rent (user_id, bike_id, rent_time)
    values('1850003', '000003', '2021-02-22 12:00:00'); ----
insert ignore into rent (user_id, bike_id, rent_time)
    values('1850004', '000004', '2021-02-22 12:00:00'); ----
insert ignore into rent (user_id, bike_id, rent_time)
    values('1850005', '000005', '2021-01-22 12:00:00'); ----
insert ignore into rent (user_id, bike_id, rent_time)
    values('1850006', '000006', '2020-12-22 12:00:00'); ----
insert ignore into rent (user_id, bike_id, rent_time)
    values('1850007', '000007', '2020-12-22 12:00:00'); ----
insert ignore into rent (user_id, bike_id, rent_time)
    values('1850008', '000008', '2020-12-22 12:00:00'); ----

insert ignore into retur (user_id, bike_id, return_time)
    values('1853790', '000001', current_timestamp); ----
insert ignore into retur (user_id, bike_id, return_time)
    values('1850002', '000002', '2021-03-22 12:20:00'); ----
insert ignore into retur (user_id, bike_id, return_time)
    values('1850003', '000003', '2021-02-22 12:20:00'); ----
insert ignore into retur (user_id, bike_id, return_time)
    values('1850004', '000004', '2021-02-22 12:20:00'); ----
insert ignore into retur (user_id, bike_id, return_time)
    values('1850005', '000005', '2021-01-22 12:20:00'); ----
insert ignore into retur (user_id, bike_id, return_time)
    values('1850006', '000006', '2020-12-22 12:20:00'); ----
insert ignore into retur (user_id, bike_id, return_time)
    values('1850007', '000007', '2020-12-22 12:20:00'); ----
insert ignore into retur (user_id, bike_id, return_time)
    values('1850008', '000008', '2020-12-22 12:20:00'); ----

insert ignore into news (news_title, news_body, news_author, news_created, news_updated, total_views)
    values('哈啰出行发布共享出行行业首份碳中和倡议书', '在今年全国两会上，碳达峰、碳中和被首次写入政府工作报告，令这一中国对世界的庄严承诺再次成为热点话题。3月10日，专业移动出行平台哈啰出行发布共享出行行业内的首份碳中和倡议书，呼吁共享出行行业、产业链合作伙伴及相关领域专家学者共商行业碳达峰、碳中和行动路线图，为实现中国“双碳”目标贡献行业力量。我国于2020年在联合国大会上明确提出，二氧化碳排放力争于2030年前达到峰值，努力争取2060年前实现碳中和。之后，碳达峰、碳中和在多个中央重要会议中高频出现，中央经济工作会议亦将“做好碳达峰、碳中和工作”作为2021年要抓好的重点任务之一。今年全国两会期间，多位代表委员就这一话题发表看法或提交建议和提案，其中，全国政协委员凌振国提出，共享出行行业的社会价值应体现在助力碳达峰目标上，这为行业可持续发展提出了新思路。此次哈啰出行面向行业发布碳中和六条倡议，在素来以低碳绿色为标签的共享出行行业尚属首次。',
     'test', '2021-03-22 12:20:00', '2021-03-22 12:20:00', 0); ----
insert ignore into news (news_title, news_body, news_author, news_created, news_updated, total_views)
    values('可爱汪先生和他的魔力小单车，超圈粉！85岁院士课后冒雨骑单车“溜了”～',
    '同济大学 公选课《科学与文化》开放直播，近10万网友涌入听讲。当晚有雨，课后85岁院士汪品先仍准备回办公室工作。大家追着送伞，老先生却说“雨不大”，骑着车“开溜”，次日又早早骑着小车现身校园可爱可敬，赞！', 'zzh',
    '2021-04-20 12:10:00', '2021-04-20 12:10:00', 0); ----
insert ignore into news (news_title, news_body, news_author, news_created, news_updated, total_views)
    values('哈啰！你来嘉定了！',
    '从2019年9月12日开始，“哈啰”单车将在嘉定校区投放，与蓝天相映，为嘉校增一抹蓝，为大家添一份便。“哈啰”单车可在支付宝上关注“哈啰”出行获取，只需芝麻信用达到650就可无须押金骑行，还可以获得一个能量球为森林付出一份力！期待着，眺望着，等待着，有“哈啰”的日子里，相信一切都会变得更美好！哈喽，我是“哈啰”！', 'test',
    '2021-03-20 12:00:00', '2021-03-20 12:00:00', 0); ----
insert ignore into news (news_title, news_body, news_author, news_created, news_updated, total_views)
    values('上海旅游节推出特别线路：骑哈啰单车感受“世界会客厅”',
    '哈啰单车在17日宣布，即日起持续一个月，上海市民和国内外游客可以骑上一辆哈啰小蓝车，打卡已由“工业锈带”变身“生活秀带”的杨浦滨江沿线地标，也可以前往洋溢着时尚和创新氛围的五角场广场、创智天地等都市新地标，感受正在创建“世界会客厅”的杨浦新魅力。杨浦区是中国近代工业的发源地之一，滨江沿线曾密布造船、纺织、发电等300多家企业。上世纪90年代，随着上海产业结构调整，很多企业关停并转，曾经的工业基地变成了“工业锈带”。2019年，自“人民城市人民建，人民城市为人民”的重要理念被提出后，杨浦滨江公共空间开发按下“快进键”，“工业锈带”变身“生活秀带”。为更好地支持杨浦滨江创建“世界会客厅”全域旅游特色示范区，依托杨浦区人民政府、上海市文旅局、市信息服务行业协会，哈啰出行联手上海杨浦滨江投资开发有限公司，将于9月22日举办“共行共美好·杨浦滨江世界会客”高峰论坛。上海众多新经济企业的掌门人、澎湃新闻等知名媒体代表将齐聚一堂，共同领略杨浦滨江会客厅的美景，并就“共建美好城市，共建美好生活”各抒己见。',
    'test',
    '2021-04-01 11:59:59', '2021-04-01 11:59:59', 0);