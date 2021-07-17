import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import LinearRegression
import datetime
# 引入自己写的包
from line_regression import *

data = pd.read_excel("Concrete_Data.xls")
# print(len(data))
req_col_names = ["Cement", "BlastFurnaceSlag", "FlyAsh", "Water", "Superplasticizer",
                 "CoarseAggregate", "FineAggregate", "Age", "CC_Strength"]
curr_col_names = list(data.columns)

mapper = {}
for i, name in enumerate(curr_col_names):
    mapper[name] = req_col_names[i]

data = data.rename(columns=mapper)
# 区分自变量和因变量，将前面所有的作为自变量，最后一个作为回归的因变量。
X = data.iloc[:,:-1]         # Features - All columns but last
y = data.iloc[:,-1]          # Target - Last Column
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=2021)
X_train_init = X_train
X_test_init = X_test
start_sklearn = datetime.datetime.now()
sc = StandardScaler()

X_train = sc.fit_transform(X_train.astype(float))
X_test = sc.transform(X_test.astype(float))

lr = LinearRegression()
lr.fit(X_train, y_train)
y_pred_lr = lr.predict(X_test)
cost_sklearn=mse(np.array(y_pred_lr),np.array(y_test))
print('sklearn-mse',cost_sklearn)

end_sklearn = datetime.datetime.now()
print('use time  ',end_sklearn-start_sklearn)


start_my = datetime.datetime.now()
x_norm , mu ,sigma = normalized(np.array(X_train_init))
theta , c = my_fit(x_norm,np.array(y_train),0.01,500)
my_ypred = predict(np.array(X_test_init),mu,sigma,theta)
cost_my=mse(np.array(my_ypred),np.array(y_test))
print('my-mse',cost_my)

end_my = datetime.datetime.now()
print('use time  ',end_my-start_my)