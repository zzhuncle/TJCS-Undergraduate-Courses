# -*- coding: utf-8 -*-
import numpy as np
# 读取txt的数据
# filename是文件名
# split是分隔符
# datatype是数据的类型
def loaddata(filename, split, dataType):
    return np.loadtxt(filename, delimiter=split, dtype=dataType)

# 画每次迭代代价的变化图
'''def plotCost(Cost_history, num_iters):
    x = np.arange(1, num_iters + 1)
    plt.plot(x, Cost_history)
    plt.xlabel(u"times")  # 注意指定字体，要不然出现乱码问题
    plt.ylabel(u"value")
    # plt.title(u"代价随迭代次数的变化", fontproperties=font)
    plt.show()'''


# 输入要打开的文件
# alpha是每次下降的长度
# iters是迭代的次数
def lineregression(filename,alpha=0.01,iters=500):
    print('begin read data')
    # 从txt中读取数据
    data = loaddata(filename,',',np.float64)
    # 得到x和y
    # x是 n*m      y是 n*1
    x = data[:,0:-1]
    y = data[:,-1]

    # 把x归一化 ， 能更快的收敛
    x_norm,mu,sigma = normalized(x)
    # 进行求解
    theta, Cost_history = my_fit(x_norm, y,alpha,iters)
    # plotCost(Cost_history, iters)
    return mu, sigma, theta  # 返回均值mu,标准差sigma,和学习的结果theta


# 测试linearRegression函数
def testLinearRegression():
    mu, sigma, theta = lineregression('data.txt',0.01, 400)
    print (u"\n计算的theta值为：\n",theta)
    print (u"\n预测结果为：%f"%predict([1230,3],mu, sigma, theta))

# 核心的函数（线性规划）

# 进行x的标准化
def normalized(X):
    X=np.array(X)
    mu = np.mean(X,0) # 求列的平均值(1*col num)
    sigma = np.std(X,0) # 求列的平均值(1*col num)
    x_norm = (X-mu)/sigma
    return x_norm,mu,sigma

# 代价函数
def Cost(X, y, theta):
    m = len(y)
    J = 0
    c=np.dot(X , theta) - y
    ctrans=np.transpose(c) 
    J = (np.dot(ctrans,c)) / (2 * m)  # 计算代价J
    return J

# 梯度下降求解
def GradientDescent(X,y,theta,alpha,iters):
    m = len(y)
    Cost_history = np.zeros((iters, 1))  # 记录每次迭代计算的代价值

    # 循环给定的迭代次数
    for i in range(iters):  
        h = np.dot(X, theta)  # 计算内积，matrix可以直接乘
        theta = theta - ((alpha / m) * (np.dot(np.transpose(X), h - y)))  # 梯度的计算
        Cost_history[i] = Cost(X, y, theta)  # 调用计算代价函数
        # print('代价为',Cost_history[i])
        # print('theta=',theta)
    return theta, Cost_history

# 输入归一化初始样本的X
# x的每一行是一个样本
# y是列向量，也可以是行向量，函数中进行转换
# theta是迭代的初始值
def my_fit(X,y,alpha=0.01,iters=500):
    row = len(y)
    x_norm = np.hstack((np.ones((row, 1)), X))  # 在x_norm前加一列1，作为常数项
    col = x_norm.shape[1]  # data的列数
    theta = np.zeros((col, 1)) #theta的初始值可以设置
    y = y.reshape(-1, 1)  # 将行向量转化为列
    # 进行梯度下降
    theta, Cost_history = GradientDescent(x_norm, y, theta, alpha, iters)
    return theta, Cost_history

# 测试学习效果（预测）
# 输入X（一个样本）行向量,不用归一化，mu, sigma, theta
# 输出y
def predict(X,mu, sigma, theta):
    result = 0
    predict = np.array(X)
    norm_predict = (predict - mu) / sigma #归一化
    # 加入常数列
    row = len(X)
    final_predict = np.hstack((np.ones((row, 1)), norm_predict))
    result = np.dot(final_predict, theta)  # 预测结果
    return result

def mse(ypred,ytrue):
    ypred = ypred.reshape(-1,1)
    ytrue = ytrue.reshape(-1,1)
    c = ypred-ytrue
    len_y=len(ytrue)*1.0
    res = 0
    for cont in c:
        res = res + cont * cont
    return res/len_y

'''if __name__ == "__main__":
    testLinearRegression()'''