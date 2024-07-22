import math
import random

data = open("./hw6/data/breast-cancer.data", "r").readlines()
for i in range(len(data)):
    data[i] = data[i].split(',')

for i in range(len(data)):
    for j in range(len(data[i])):
        if data[i][j] == "?":
            c = dict()
            for k in range(len(data)):
                if data[k][j] == "?":
                    continue
                if data[k][j] in c:
                    c[data[k][j]] += 1
                else:
                    c[data[k][j]] = 1
            m = 0
            v = 0
            for item in c.items():
                if item[1] > m:
                    v = item[0]
                    m = item[1]

            data[i][j] = v


def entropy(probs):
    e = 0
    for i in probs:
        if i > 0:
            e += -i*math.log2(i)
    return e


possible_vals = []
for i in range(len(data[0])):
    possible_vals.append(set([j[i] for j in data]))


def info_gain(dataset, attr):
    ig = 0
    cnts = dict()
    for i in possible_vals[0]:
        cnts[i] = 0
    for i in dataset:
        cnts[i[0]] += 1
    ig += entropy([i[1]/len(dataset) for i in cnts.items()])
    cnts = dict()
    for i in possible_vals[attr]:
        cnts[i] = dict()
        for j in possible_vals[0]:
            cnts[i][j] = 0
    for i in dataset:
        cnts[i[attr]][i[0]] += 1

    for i in cnts.items():
        cur = []
        for j in i[1].items():
            cur.append(j)
        s = cur[0][1]+cur[1][1]
        if s > 0:
            ig -= s/len(dataset)*entropy([cur[0][1]/s, cur[1][1]/s])
    return ig


def most_common_class(dataset):
    cnts = dict()
    for i in possible_vals[0]:
        cnts[i] = 0
    for i in dataset:
        cnts[i[0]] += 1
    best = 0
    ans = ""
    for i in cnts.items():
        if i[1] > best:
            ans = i[0]
            best = i[1]
    return ans, best


def build(dataset, used, dominant, K):
    mc, cnt = most_common_class(dataset)
    if cnt != len(dataset)/len(possible_vals[0]):
        dominant = mc
    if len(dataset) < K or cnt == len(dataset) or len(used) == len(data[0])-1:
        return [dominant]
    best = -1e9
    pos = -1
    for i in range(1, len(dataset[0])):
        if (i in used) == False:
            cur = info_gain(dataset, i)
            if cur > best:
                best = cur
                pos = i

    cur = [pos, []]
    used.add(pos)
    for v in possible_vals[pos]:
        new_dataset = []
        for d in dataset:
            if d[pos] == v:
                new_dataset.append(d)
                
        # if len(new_dataset)==0:
        #     new_dataset=dataset[:]
        
        cur[1].append([v, build(new_dataset, used, dominant, K)])

    used.remove(pos)
    return cur


def predict(tree, d):
    cur = tree
    while True:
        if len(cur) == 1:
            return cur[0]
        attr = cur[0]
        for j in cur[1]:
            if j[0] == d[attr]:
                cur = j[1]


indices = [i for i in range(len(data))]
random.shuffle(indices)

folds = 10
avg_acc = 0
for i in range(folds):
    train = []
    test = []
    for j in range(len(data)):
        if j % folds == i:
            test.append(data[indices[j]])
        else:
            train.append(data[indices[j]])

    tree = build(train, set(), most_common_class(data), 10)
    acc = 0
    for x in test:
        cur = predict(tree, x)
        if cur == x[0]:
            acc += 1
    acc /= len(test)
    print(acc)

    avg_acc += acc

avg_acc /= folds
print("average accuracy: ", avg_acc)
