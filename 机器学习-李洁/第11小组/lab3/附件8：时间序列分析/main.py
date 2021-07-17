import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import warnings
import os

df = pd.read_csv('Live.csv', encoding='utf-8')
df.drop_duplicates(inplace=True) # 去除重复

df.drop(labels=['Column1', 'Column2', 'Column3','Column4'], axis=1, inplace=True) # 去除无用的列
df.drop('status_id', axis=1, inplace=True)
df['status_type_isvideo'] = df['status_type'].map(lambda x:1 if(x=='video') else 0) # 是否是视频
df.drop('status_type', axis=1, inplace=True)
df['status_published'] = pd.to_datetime(df['status_published']) # 发布时间格式

# 时间序列分析
df['year'] = df['status_published'].dt.year
df['month'] = df['status_published'].dt.month
df['dayofweek'] = df['status_published'].dt.dayofweek
df['hour'] = df['status_published'].dt.hour

reaction = ['num_reactions', 'num_comments', 'num_shares', 'num_likes', 'num_loves', 'num_wows', 'num_hahas','num_sads', 'num_angrys']

# 以2016年为界进行分析
before2016 = df[df['year']<=2015]
after2016 = df[df['year']>2015]

''' 2016年前 '''
# 年份分析
before2016.groupby('year').sum()[reaction].plot(figsize=(10,5))
plt.figure(1)
before2016[before2016['status_type_isvideo']==0].groupby('year').sum()[reaction].plot(figsize=(10,5), title='photo content')
plt.figure(2)
before2016[before2016['status_type_isvideo']==1].groupby('year').sum()[reaction].plot(figsize=(10,5), title='video content')
plt.show()

# 月份分析
plt.figure(0)
before2016[before2016['status_type_isvideo']==0].groupby('month').sum()[reaction].plot(figsize=(10,5), title='photo content')
plt.figure(1)
before2016[before2016['status_type_isvideo']==1].groupby('month').sum()[reaction].plot(figsize=(10,5), title='video content')
plt.show()

# 星期分析
plt.figure(0)
before2016[before2016['status_type_isvideo']==0].groupby('dayofweek').sum()[reaction].plot(figsize=(10,5), title='photo content')
plt.figure(1)
before2016[before2016['status_type_isvideo']==1].groupby('dayofweek').sum()[reaction].plot(figsize=(10,5), title='video content')
plt.show()

# 小时分析
plt.figure(0)
before2016[before2016['status_type_isvideo']==0].groupby('hour').sum()[reaction].plot(figsize=(10,5), title='photo content')
plt.figure(1)
before2016[before2016['status_type_isvideo']==1].groupby('hour').sum()[reaction].plot(figsize=(10,5), title='video content')
plt.show()


''' 2016年后 '''
# 年份分析
after2016.groupby('year').sum()[reaction].plot(figsize=(10,5))
plt.figure(0)
after2016[after2016['status_type_isvideo']==0].groupby('year').sum()[reaction].plot(figsize=(10,5), title='photo content')
plt.figure(1)
after2016[after2016['status_type_isvideo']==1].groupby('year').sum()[reaction].plot(figsize=(10,5), title='video content')
plt.show()

# 月份分析
plt.figure(0)
after2016[after2016['status_type_isvideo']==0].groupby('month').sum()[reaction].plot(figsize=(10,5), title='photo content')
plt.figure(1)
after2016[after2016['status_type_isvideo']==1].groupby('month').sum()[reaction].plot(figsize=(10,5), title='video content')
plt.show()

# 星期分析
plt.figure(0)
after2016[after2016['status_type_isvideo']==0].groupby('dayofweek').sum()[reaction].plot(figsize=(10,5), title='photo content')
plt.figure(1)
after2016[after2016['status_type_isvideo']==1].groupby('dayofweek').sum()[reaction].plot(figsize=(10,5), title='video content')
plt.show()

# 小时分析
plt.figure(0)
after2016[after2016['status_type_isvideo']==0].groupby('hour').sum()[reaction].plot(figsize=(10,5), title='photo content')
plt.figure(1)
after2016[after2016['status_type_isvideo']==1].groupby('hour').sum()[reaction].plot(figsize=(10,5), title='video content')
plt.show()
