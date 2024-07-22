import random
import math

data = open("./hw5/dataset.data", "r").readlines()
for i in range(len(data)):
    data[i] = data[i].split(',')
    for j in range(len(data[i])):
        if data[i][j] == 'republican' or data[i][j] == 'y':
            data[i][j] = 0
        elif data[i][j] == 'democrat' or data[i][j] == 'n':
            data[i][j] = 1
        else:
            data[i][j] = 2


for i in range(len(data)):
    for j in range(len(data[i])):
        if data[i][j] == 2:
            c = [0, 0]
            for k in range(len(data)):
                if data[k][j] != 2 and data[k][0] == data[i][0]:
                    c[data[k][j]] += 1
            if c[0] > c[1]:
                data[i][j] = 0
            else:
                data[i][j] = 1


def calc_probs(data):
    n = len(data)
    k = len(data[0])
    p = []

    for i in range(k):
        cnt = [0]*2
        for j in range(2):
            cnt[j] = [0]*2

        for j in range(n):
            cnt[data[j][i]][data[j][0]] += 1

        p.append(cnt)

    alpha = 1
    for i in range(1, k):
        for j in range(2):
            for k in range(2):
                p[i][j][k] += alpha
                p[i][j][k] /= (p[0][k][k]+2*alpha)

    for i in range(2):
        p[0][i][i] += alpha
        p[0][i][i] /= (n+2*alpha)

    return [p[0][0][0], p[0][1][1]], p[1:]


def predict(class_probs, cond_probs, data):
    p = [math.log2(class_probs[0]), math.log2(class_probs[1])]
    for i in range(len(data)):
        p[0] += math.log2(cond_probs[i][data[i]][0])
        p[1] += math.log2(cond_probs[i][data[i]][1])
    if p[0] > p[1]:
        return 0
    else:
        return 1


indexes = [i for i in range(len(data))]
random.shuffle(indexes)

trials = 10
avgacc = 0
for i in range(trials):
    train = []
    test = []
    for j in range(len(data)):
        if j % trials == i:
            test.append(data[indexes[j]])
        else:
            train.append(data[indexes[j]])
    class_probs, cond_probs = calc_probs(train)
    acc = 0
    for x in test:
        cur = predict(class_probs, cond_probs, x[1:])
        if cur == x[0]:
            acc += 1
    acc /= len(test)
    print(acc)

    avgacc += acc

avgacc /= trials
print("average accuracy: ", avgacc)
