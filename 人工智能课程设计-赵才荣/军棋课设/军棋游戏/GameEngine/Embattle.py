import numpy as np

__all__ = ['load_embattle']


def load_embattle(txt_path):
    with open(txt_path, 'r') as f:
        result = f.readlines()
        for i in range(len(result)):
            result[i] = result[i].replace(',', ' ').replace('\t', ' ').replace('\n', ' ')
            # print(result[i])
            result[i] = result[i].split(' ')
            result[i] = [j for j in result[i] if j != ' ' and j != '']
            result[i] = [eval(j) for j in result[i]]
    return np.array(result)
