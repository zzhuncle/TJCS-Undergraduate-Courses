import tensorflow as tf
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten
from keras.layers import Conv2D, MaxPooling2D, ZeroPadding2D, GlobalMaxPooling2D
import keras
from keras.datasets import cifar10
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
import time

classes_num = 10
batch_size = 64
epochs_num = 50
activation_fun = "relu"
drop_prob = 0.5 #全连接drop out的概率
drop_prob_pool1 = 0.25

def deep_cnn():
    model = Sequential()
    # 增加一个卷积层，通道数为32，卷积核大小为3*3*3，设置输入为32*32*3,激活函数为relu
    model.add(Conv2D(32, (3, 3), padding='same', input_shape=(32, 32, 3), activation=activation_fun))
    model.add(Conv2D(32, (3, 3), padding='same', activation=activation_fun))
    model.add(Conv2D(48, (3, 3), padding='same', activation=activation_fun))
    model.add(Conv2D(48, (3, 3), padding='same', activation=activation_fun))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    # 池化
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Dropout(drop_prob_pool1))

    model.add(Conv2D(80, (3, 3), padding='same', activation=activation_fun))
    model.add(Conv2D(80, (3, 3), padding='same', activation=activation_fun))
    model.add(Conv2D(80, (3, 3), padding='same', activation=activation_fun))

    model.add(MaxPooling2D(pool_size=(2, 2)))

    model.add(Conv2D(128, (3, 3), padding='same', activation=activation_fun))
    model.add(Conv2D(128, (3, 3), padding='same', activation=activation_fun))
    model.add(Conv2D(128, (3, 3), padding='same', activation=activation_fun))

    model.add(GlobalMaxPooling2D())
    model.add(Dropout(drop_prob_pool1))

    model.add(Dense(500, activation=activation_fun))
    model.add(Dropout(drop_prob_pool1))
    model.add(Dense(classes_num))
    model.add(Activation('softmax'))

    opt = keras.optimizers.Adam(lr=0.0001)
    model.compile(loss='categorical_crossentropy', optimizer=opt, metrics=['accuracy'])

    return model

def train():
    # 数据载入
    (x_train, y_train), (x_test, y_test) = cifar10.load_data()

    # 多分类标签生成
    y_train = keras.utils.to_categorical(y_train, classes_num)
    y_test = keras.utils.to_categorical(y_test, classes_num)
    # 生成训练数据
    x_train = x_train.astype('float32')
    x_test = x_test.astype('float32')
    x_train /= 255
    x_test /= 255

    model = deep_cnn()
    hist = model.fit(x_train, y_train, batch_size=batch_size, epochs=epochs_num, validation_split=0.1,verbose=2, shuffle=True)
    model.save('./deep_cnn/cifar10_model.hdf5')
    model.save_weights('./deep_cnn/cifar10_model_weight.hdf5')

    hist_dict = hist.history
    print("train acc:")
    print(hist_dict['accuracy'])
    print("validation acc:")
    print(hist_dict['val_accuracy'])

    train_acc = hist.history['accuracy']
    val_acc = hist.history['val_accuracy']
    train_loss = hist.history['loss']
    val_loss = hist.history['val_loss']

    # 绘图
    epochs = range(1, len(train_acc) + 1)
    plt.plot(epochs, train_acc, 'bo', label='Training acc')
    plt.plot(epochs, val_acc, 'r', label='Validation acc')
    plt.title('Training and validation accuracy')
    plt.legend()
    plt.savefig("accuracy.png")
    plt.figure()  # 新建一个图
    plt.plot(epochs, train_loss, 'bo', label='Training loss')
    plt.plot(epochs, val_loss, 'r', label='Validation loss')
    plt.title('Training and validation loss')
    plt.legend()
    plt.savefig("loss.png")
    res = model.predict(x_test)
    num = 0
    for i in range(len(res)):
        t = np.argmax(res[i])
        if y_test[i][t] > 0.9:
            num = num + 1
    print('test acc', num * 1.0 / len(res))

print('begin')
t0 =time.time()
train()
print('end')
t1=time.time()
print('use : ',t1-t0)
