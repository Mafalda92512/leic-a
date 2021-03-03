# -*- coding: utf-8 -*-
"""
Grupo al001
Catarina Goncalves id #92437
Mafalda Serafim id #92512
"""

import numpy as np

def addBranch(list, feature, l, D, Y, gains, noise):

    equal = True
    i = 0
    k = Y[0]
    if len(D) == 1:
        return Y[0]
    i = 0
    while i < len(Y):
        if k != Y[i]:
            equal = False
            i = len(Y)
        i = i + 1
    if equal == True:
        list = Y[0]
    else:

        i = 0
        y1 = []
        y2 = []
        d1 = []
        d2 = []
        while i < len(D):
            if D[i][feature] == 0:
                y1.append(Y[i])
                d1.append(D[i])
            elif D[i][feature] == 1:
                y2.append(Y[i])
                d2.append(D[i])
            i = i+1
        index = 0
        max = -1


        gains_aux1 = calculateGains(d1, y1, noise)
        gains_aux2 = calculateGains(d2, y2, noise)

        gains_aux1[feature] = -1
        gains_aux2[feature] = -1

        i = 0
        index1 = -1
        index2 = -1
        while i < len(gains_aux1):
            if gains_aux1[i] > max:
                max = gains_aux1[i]
                index1 = i
            i = i + 1
        i = 0
        max = -1
        while i < len(gains_aux2):
            if gains_aux2[i] > max:
                max = gains_aux2[i]
                index2 = i
            i = i + 1
        if d1 != [] and d2 != []:
            list = [feature, addBranch(list, index1, l, d1, y1, gains_aux1, noise), addBranch(list, index2, l, d2, y2, gains_aux2, noise)]
        else:
            if d1 == []:
                list = addBranch(list, index2, l, d2, y2, gains_aux2, noise)
            else:
                list = addBranch(list, index1, l, d1, y1, gains_aux1, noise)

    return list

def calculateGains(D, Y, noise):
    gains = []
    j = 0
    while j < len(D[0]):
        p = 0
        n = 0
        for i in Y:
            if i == 0:
                n = n + 1
            else:
                p = p + 1
        if p!= 0 and n != 0:
            i_init = -(p/(p+n))*np.log2(p/(p+n)) - (n/(p+n))*np.log2(n/(p+n))
        else:
            if p == 0 and n!=0:
                i_init = - (n/(p+n))*np.log2(n/(p+n))
            elif n == 0 and p!=0:
                i_init = -(p/(p+n))*np.log2(p/(p+n))
            else:
                i_init=0
        y1 = []
        y2 = []
        i = 0
        while i < len(D):
            if D[i][j] == 0:
                y1.append(Y[i])
            elif D[i][j] == 1:
                y2.append(Y[i])
            i = i+1
        # quando o ramo e positivo (direita)
        ps = 0
        ns = 0
        for i in y1:
            if i == 0:
                ns = ns + 1
            else:
                ps = ps + 1


        if ps != 0 and ns != 0:
            i_p = -(ps/(ps+ns))*np.log2(ps/(ps+ns)) - (ns/(ps+ns))*np.log2(ns/(ps+ns))
        else:
            if ps == 0 and ns!= 0:
                i_p = - (ns/(ps+ns))*np.log2(ns/(ps+ns))
            elif ns == 0 and ps != 0:
                i_p = -(ps/(ps+ns))*np.log2(ps/(ps+ns))
            else:
                i_p = 0
        # quando o ramo e negativo (esquerda)
        nn = 0
        pn = 0
        for i in y2:
            if i == 0:
                nn = nn + 1
            else:
                pn = pn + 1
        if pn != 0 and nn != 0:
            i_n = -(pn/(pn+nn))*np.log2(pn/(pn+nn)) - (nn/(pn+nn))*np.log2(nn/(pn+nn))
        else:
            if pn == 0 and nn != 0:
                i_n = - (nn/(pn+nn))*np.log2(nn/(pn+nn))
            elif nn == 0 and pn != 0:
                i_n = -(pn/(pn+nn))*np.log2(pn/(pn+nn))
            else:
                i_n = 0
        res = ((ps+ns)/(ps+pn+ns+nn))*i_p + ((pn+nn)/(ps+pn+ns+nn))*i_n
        gain = i_init - res
        if noise == 0.1:
            if i_p == i_n:
                gain = 0.000000000001
        gains.append(gain)
        j = j + 1
    return gains

def shortList(list1, feature):
    if list1 == 0 or list1 == 1 or list1 == [feature, 0, 1] or list1 == [feature, 1, 0]:
        return list1
    elif list1 == [feature, 0, 0]:
        return 0
    elif list1 == [feature, 1, 1]:
        return 1
    elif list1[1] == list1[2]:
        return shortList(list1[1], feature+1)
    elif isinstance(list1[1], list) and isinstance(list1[2], list) and list1[1][1] == list1[2][1]:
        return [list1[1][0], list1[1][1], [list1[0], list1[1][2], list1[2][2]]]
    return [list1[0], shortList(list1[1], feature+1), shortList(list1[2], feature+1)]


def clean(D, Y):

    j = 0
    while j < len(D):
        k = D[j]
        d_aux = []
        y_aux = []
        i = j
        while i < len(D):
            if D[i] == k and i != j:
                d_aux.append(D[i])
                y_aux.append(Y[i])
                D.pop(i)
                Y.pop(i)

            elif D[i] == k:
                d_aux.append(D[i])
                y_aux.append(Y[i])
                i = i + 1
            else:
                i = i + 1
        if len(d_aux) > 1:
            num1 = 0
            num0 = 0
            m = 0
            while m < len(y_aux):
                if y_aux[m] == 0:
                    num0 = num0 + 1
                else:
                    num1 = num1 + 1
                m = m + 1
            if num0 > num1:
                Y[j] = 0
            elif num0 < num1:
                Y[j] = 1
            else:
                Y[j] = 1
        j = j + 1


    return D, Y


def createdecisiontree(D, Y, noise):

    helpD = np.multiply(D, 1)
    helpY = np.multiply(Y, 1)
    helpD = np.array(helpD).tolist()
    helpY = np.array(helpY).tolist()
    list = []
    l = len(D[0])

    feature = 0

    if noise:
        helpD, helpY = clean(helpD, helpY)
    gains = calculateGains(helpD, helpY, noise) # D, Y -> helpD, helpY
    index = 0
    max = 0
    i = 0
    while i < len(gains):
        if gains[i] > max:
            max = gains[i]
            index = i
        i = i + 1
    list = addBranch(list, index, l, helpD, helpY, gains, noise)

    if list == 0:
        list = [0, 0, 0]
    if list == 1:
        list = [0, 1, 1]
    list_aux = []
    while list != list_aux:
        list_aux = list
        if list == 0 or list == 1:
            break
        else:
            list = shortList(list_aux, list[0])
    if list == 0:
        list = [0, 0, 0]
    if list == 1:
        list = [0, 1, 1]

    return list
