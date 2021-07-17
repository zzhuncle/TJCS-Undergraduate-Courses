# Todo:
# 看一看各个类别和视频、照片、文本有什么联系
'''
原型聚类
    [x] sklearn.cluster.KMeans
    sklearn.cluster.SpectralClustering 谱聚类
    sklearn.cluster.MiniBatchKMeans

层次聚类
    [x] sklearn.cluster.AgglomerativeClustering
    sklearn.cluster.Birch

密度聚类
    [x] sklearn.cluster.DBSCAN
    sklearn.cluster.MeanShift
    [x] sklearn.mixture.GaussianMixture

性能度量
'''
# video 0 | photo 1 | link 2 | status 3
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
from sklearn.cluster import SpectralClustering
from sklearn.cluster import MiniBatchKMeans
from sklearn.cluster import Birch
from sklearn.cluster import MeanShift
from sklearn.cluster import KMeans
from mpl_toolkits.mplot3d import Axes3D

# 支持中文
plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False  # 用来正常显示负号

names = ['反应数量', '评论数量', '分享数量', '喜欢数量', '爱心表情', '惊讶表情', '哈哈表情', '悲伤表情',
         '愤怒表情', '表情数量', '好评数量', '差评数量', '原始类别']
colors = ['purple', 'blue', 'green', 'cyan', 'yellow']
clusters = 3
classes = ['video', 'photo', 'link', 'status']

'''
# 流量程度 3
num_comment | num_share | num_reaction

# 传统和现代程度 2
num_likes | num_loves + num_wows + num_hahas + num_sads + num_angrys

# 卖家信誉 2 
num_share | num_likes

# 本土品牌与国际品牌 3
num_comment | num_likes + num_loves + num_wows + num_hahas | num_sad + num_angrys

# 受众：男性、女性 2
num_share | num_loves + num_wows + num_hahas + num_sads + num_angrys

'''

# 数据读入64
data = pd.read_csv ('Live_20210128.csv',index_col = 0)
data = data.dropna(axis = 1) # 去除后四列无用信息

data['num_emotion'] = data['num_loves'] + data['num_wows'] + data['num_hahas'] + data['num_sads'] + data['num_angrys']
data['num_good'] = data['num_likes'] + data['num_loves'] + data['num_wows'] + data['num_hahas']
data['num_bad'] = data['num_sads'] + data['num_angrys']

# 将状态类型更改为整数
data['class'] = data['status_type']
data['class'][data['status_type'] == 'video'] = 0
data['class'][data['status_type'] == 'photo'] = 1
data['class'][data['status_type'] == 'link'] = 2
data['class'][data['status_type'] == 'status'] = 3

X = data.iloc[:, 2:]
X = np.array(X)

def plot_2d(x, y, clusters, labels, titl):
    plt.figure(figsize=(14, 6))
    plt.subplot(1, 2, 1)
    for i in range(clusters):
        plt.scatter(x[y==i,0], x[y==i,1], s=50, c=colors[i], label=f'cluster{i}')
    plt.title(titl)
    plt.xlabel(labels[0])
    plt.ylabel(labels[1])
    plt.legend()
    plt.subplot(1, 2, 2)
    for i in range(4):
        plt.scatter(x[X[:,12]==i,0], x[X[:,12]==i,1], s=50, c=colors[i], label=classes[i])
    plt.title('原始类别')
    plt.xlabel(labels[0])
    plt.ylabel(labels[1])
    plt.legend()
    plt.show()

def plot_3d(x, y, clusters, labels, titl):
    fig = plt.figure(figsize=(14, 6))
    ax = fig.add_subplot(121, projection='3d')
    for i in range(clusters):
        ax.scatter(x[y == i, 0], x[y == i, 1], x[y == i, 2], s=50, c=colors[i], label=f'cluster{i}')
    plt.title(titl)
    ax.set_xlabel(labels[0])
    ax.set_ylabel(labels[1])
    ax.set_zlabel(labels[2])
    plt.legend()

    ax = fig.add_subplot(122, projection='3d')
    for i in range(4):
        ax.scatter(x[X[:,12]==i,0], x[X[:,12]==i,1], x[X[:,12]==i,2], s=50, c=colors[i], label=classes[i])
    plt.title('原始类别')
    ax.set_xlabel(labels[0])
    ax.set_ylabel(labels[1])
    ax.set_zlabel(labels[2])
    plt.legend()
    plt.show()

# 流量程度 谱聚类
'''
  谱聚类是从图论中演化出来的算法，后来在聚类中得到了广泛的应用。它的主要思想是把所有的数据看做空间中的点，
这些点之间可以用边连接起来。距离较远的两个点之间的边权重值较低，而距离较近的两个点之间的边权重值较高，
通过对所有数据点组成的图进行切图，让切图后不同的子图间边权重和尽可能的低，而子图内的边权重和尽可能的高，
从而达到聚类的目的。
  需要调参的是相似矩阵建立相关的参数和聚类类别数目.
'''
x1 = np.empty((X.shape[0], 3), np.int64)
x1[:,:] = X[:,0:3]
labels = names[0:3]
titl = '流量程度'
# model = SpectralClustering(n_clusters = clusters)
model = KMeans(n_clusters=clusters, init='k-means++', max_iter=300, n_init=10, random_state=0)
y1 = model.fit_predict(x1)
plot_3d(x1, y1, clusters, labels, titl)

# 传统和现代程度 Mini Batch K-means聚类
'''
  Mini Batch K-Means算法是K-Means算法的一种优化变种，采用小规模的数据子集（每次训练使用的数据集是在训练算法的时候随机抽取的
数据子集）减少计算时间，同时试图优化目标函数；Mini Batch K-Means算法可以减少 K-Means 算法的收敛时间，而且产生的结果和标准
K-Means 算法相近。
'''
batch_size = 100
x2 = np.empty((X.shape[0], 2), np.int64)
x2[:,0] = X[:,3]
x2[:,1] = X[:,9]
labels = [names[3], names[9]]
titl = '传统和现代程度'
# model = MiniBatchKMeans(init='k-means++', n_clusters=clusters, batch_size=batch_size)
model = KMeans(n_clusters=clusters, init='k-means++', max_iter=300, n_init=10, random_state=0)
y2 = model.fit_predict(x2)
plot_2d(x2, y2, clusters, labels, titl)

# 卖家信誉
'''
  Birch聚类，BIRCH的全称是利用层次方法的平衡迭代规约和聚类，它是用层次方法来聚类和规约数据。
BIRCH只需要单遍扫描数据集就能进行聚类，利用了一个树结构来帮助我们快速的聚类，这个树结构类似于
平衡B+树，一般将它称之为聚类特征树。这颗树的每一个节点是由若干个聚类特征组成。
'''
x3 = np.empty((X.shape[0], 2), np.int64)
x3[:,0] = X[:,2]
x3[:,1] = X[:,3]
labels = [names[2], names[3]]
titl = '卖家信誉'
# model = Birch(n_clusters=clusters)
model = KMeans(n_clusters=clusters, init='k-means++', max_iter=300, n_init=10, random_state=0)
y3 = model.fit_predict(x2)
plot_2d(x3, y3, clusters, labels, titl)

# 本土品牌与国际品牌
'''
  MeanShift聚类，均值漂移聚类涉及到根据特征空间中的实例密度来寻找和调整质心。对离散数据证明了
递推平均移位程序收敛到最接近驻点的基础密度函数，从而证明了它在检测密度模式中的应用。它是通过 
MeanShift 类实现的，主要配置是“带宽”超参数。
'''
# 均值漂移聚类
x4 = np.empty((X.shape[0], 3), np.int64)
x4[:,0] = X[:,1]
x4[:,1] = X[:,10]
x4[:,2] = X[:, 11]
labels = [names[1], names[10], names[11]]
titl = '本土品牌和国际品牌'
# model = MeanShift(n_clusters=clusters)
model = KMeans(n_clusters=2, init='k-means++', max_iter=300, n_init=10, random_state=0)
y4 = model.fit_predict(x4)
plot_3d(x4, y4, 2, labels, titl)

# 受众：男性、女性
x5 = np.empty((X.shape[0], 2), np.int64)
x5[:,0] = X[:,2]
x5[:,1] = X[:,9]
labels = [names[2], names[9]]
titl = '受众性别'
model = KMeans(n_clusters=2, init='k-means++', max_iter=300, n_init=10, random_state=0)
y5 = model.fit_predict(x5)
plot_2d(x5, y5, 2, labels, titl)
