from datetime import datetime
from datetime import time

import nltk
import string

import numpy as np
from nltk.corpus import stopwords
import pymorphy2
import matplotlib.pyplot as plt
from matplotlib import dates


def data_filtering(file_text):
    stop_symbol = string.punctuation + '…«»0123456789qwerrtyuioplkjhgfdsazxcvbnmQWERTYUIOKLPJHGFDSAZXCVBNM￼₽󾓬'
    line_clear = [i for i in file_text if (i not in stop_symbol)]
    file_text = ""
    for i in range(len(line_clear)):
        file_text += line_clear[i]
    tokens = nltk.word_tokenize(file_text)
    tokens = [morph.parse(i)[0].normal_form for i in tokens]
    stop_words = stopwords.words('russian')
    tokens = [i for i in tokens if ((i not in stop_words) & (len(i) > 3) & (len(i) < 12))]  # return the number of elements
    return tokens


# preparing tweets and throwing in an array of tweets
def clear_twitter(file_text):
    list_twit = file_text.split('\n')
    list_twit = [i for i in list_twit if (i != '')]
    token = []
    for i in range(len(list_twit)):
        twit = ''
        token_twit = data_filtering(list_twit[i])  # the first function for each tweet is launched
        for j in range(len(token_twit)):
            twit = twit + token_twit[j] + ' '
            token.append(token_twit[j])
        list_twit[i] = twit
    tokens_one = list(set(token))
    return tokens_one, list_twit


def frequency(twits):  # frequency
    words = set()
    for word in twits:
        words.add(word)

    words = list(words)
    new_arr = list()
    numb_twits = len(twits)

    with open('frequency.txt', 'w', encoding='utf-8') as file:
        for wrd in range(len(words)):
            wrd_count = 0
            for twit in range(len(twits)):
                if words[wrd] in twits[twit]:
                    wrd_count += 1
            new_arr.append((words[wrd], wrd_count))
        new_arr.sort(key=lambda a: a[1], reverse=True)
        for el in new_arr:
            file.write('{}-{}-{}%\n'.format(el[0], el[1], round(el[1] / numb_twits * 100, 2)))


def twits_length(data):  # number of words in tweets
    file_len = list()
    lens = set()
    numb_twits = len(data)

    for twit in data:
        ln = len(twit)

        if ln in lens:
            continue
        else:
            lens.add(ln)
            numb_twits_with = 0
            for tw in data:
                if len(tw) == ln:
                    numb_twits_with += 1
            file_len.append((ln, numb_twits_with, round(numb_twits_with / numb_twits * 100, 2)))
    file_len.sort(key=lambda a: a[1], reverse=True)

    with open('twits_length.txt', 'w', encoding='utf-8') as file:
        for ln in file_len:
            file.write('{}-{}-{}%\n'.format(ln[0], ln[1], ln[2]))


def adj_rating():
    # создаем список
    with open('estimations.txt', 'r', encoding='utf-8-sig') as file:
        arr = list()
        for line in file.readlines():
            arr.append(tuple(line[:len(line) - 1].rstrip().split(' ')))
    dc = dict(arr)

    file_frequency = open('frequency.txt', 'r', encoding='utf-8-sig')
    positive_words, negative_words = list(), list()

    for line in file_frequency.readlines():
        p = morph.parse(line.split('-')[0])[0]
        tag_word = p.tag.POS
        if dc.get((line.split('-')[0])) is not None:
            if int(dc.get(line.split('-')[0])) == 0:
                continue
            if int(dc.get(line.split('-')[0])) == 1 and len(positive_words) < 5 and tag_word == 'ADJF':
                positive_words.append(line[:len(line) - 1])
                continue
            if int(dc.get(line.split('-')[0])) == -1 and len(negative_words) < 5 and tag_word == 'ADJF':
                negative_words.append(line[:len(line) - 1])
                continue
            if len(positive_words) == 5 and len(negative_words) == 5:
                break

    positive_words.sort(key=lambda a: int(a.split('-')[1]), reverse=True)
    negative_words.sort(key=lambda a: int(a.split('-')[1]), reverse=True)

    with open('adjectives.txt', 'w', encoding='utf-8') as file:
        file.write('Top-5 Positive:\n')
        for ln in positive_words:
            file.write(ln + '\n')

        file.write('\nTop-5 Negative:\n')
        for ln in negative_words:
            file.write(ln + '\n')

        # Chart for TOP negative or positive
        figure = plt.figure(figsize=(11, 8))
        first_bar = figure.add_axes([0.555, 0.325, 0.365, 0.352273])
        first_bar.set_title('Top 5 positive')
        second_bar = figure.add_axes([0.125, 0.325, 0.365, 0.352273])
        second_bar.set_title('Top 5 negative')
        labels1 = [el.split('-')[0] for el in positive_words]
        labels2 = [el.split('-')[0] for el in negative_words]
        width = 0.85
        positive = [float(el.split('-')[2][:-1]) for el in positive_words]
        negative = [float(el.split('-')[2][:-1]) for el in negative_words]
        first_bar.bar(labels1, positive, width, label='Positive')
        second_bar.bar(labels2, negative, width, label='Negative')
        first_bar.set_xticklabels(labels1, fontsize=8)
        second_bar.set_xticklabels(labels2, fontsize=8)
        plt.show()


def _sentimenting_twits(data):  # preparing data over time [date, time, tweet score]
    data = data[:]
    with open('data.txt', 'r', encoding='utf-8') as file:
        arr = file.read().splitlines()[::2]
        arr = [el.split(' ')[1] for el in arr if len(el.split()[2:]) != 0]
    for i in range(len(arr)):
        el = arr[i].split(':')
        arr[i] = time(int(el[0]), int(el[1]))
    with open('estimations.txt', 'r', encoding='utf-8-sig') as file:
        arr_mark = list()
        for line in file.readlines():
            arr_mark.append(tuple(line[:len(line) - 1].rstrip().split(' ')))
        dc = dict(arr_mark)
    for i in range(len(data)):
        sm = 0
        for j in data[i]:
            if dc.get(j) is not None:
                sm += int(dc.get(j))
        if -1 <= sm <= 1:
            data[i] = ['0']
        elif sm < -1:
            data[i] = ['-1']
        else:
            data[i] = ['1']

        data[i].insert(0, arr[i])

    return data


def first_rule(data):  # the first rule for classifying tweets (the sum of ratings)
    good = 0
    bad = 0
    neutral = 0
    num_twits = len(data)
    with open('estimations.txt', 'r', encoding='utf-8-sig') as file:
        arr = list()
        for line in file.readlines():
            arr.append(tuple(line[:len(line) - 1].rstrip().split(' ')))

        dc = dict(arr)

    for i in range(len(data)):
        sm = 0
        for j in data[i]:
            if dc.get(j) is not None:
                sm += int(dc.get(j))

        if -1 <= sm <= 1:
            neutral += 1
        elif sm < -1:
            bad += 1
        else:
            good += 1

    _chart_for_rule([good / num_twits * 100, bad / num_twits * 100, neutral / num_twits * 100],
                    'Sum of rating (1st rule)')

    with open('classifications.txt', 'a', encoding='utf-8') as file:
        file.write('First rule\n')
        file.write('Good-{}-{}%\n'.format(good, round(good / num_twits * 100, 3)))
        file.write('Bad-{}-{}%\n'.format(bad, round(bad / num_twits * 100, 3)))
        file.write('Neutral-{}-{}%\n'.format(neutral, round(neutral / num_twits * 100, 3)))
        file.write('~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n')


def second_rule(data):  # the second rule of classification of tweets (the largest share)
    good = 0
    bad = 0
    neutral = 0
    num_twits = len(data)

    with open('estimations.txt', 'r', encoding='utf-8-sig') as file:
        arr = list()
        for line in file.readlines():
            arr.append(tuple(line[:len(line) - 1].rstrip().split(' ')))
        dc = dict(arr)

    for i in range(len(data)):
        g = 0
        b = 0
        n = 0
        for j in data[i]:
            if dc.get(j) is not None:
                if int(dc.get(j)) == 0:
                    n += 1
                elif int(dc.get(j)) == 1:
                    g += 1
                else:
                    b += 1
        if dc.get(j) is not None:
            if n >= b and n >= g:
                neutral += 1
            elif g >= b and g >= n:
                good += 1
            else:
                bad += 1

    _chart_for_rule([good / num_twits * 100, bad / num_twits * 100, neutral / num_twits * 100], 'Largest share '
                                                                                                '(2nd rule)')
    with open('classifications.txt', 'a', encoding='utf-8') as file:
        file.write('Second rule\n')
        file.write('Good-{}-{}%\n'.format(good, round(good / num_twits * 100, 3)))
        file.write('Bad-{}-{}%\n'.format(bad, round(bad / num_twits * 100, 3)))
        file.write('Neutral-{}-{}%\n'.format(neutral, round(neutral / num_twits * 100, 3)))
        file.write('~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n')


# chart for rule
def _chart_for_rule(params, title):
    categories = ['Good', 'Bad', 'Neutral']
    y = np.arange(len(categories))
    plt.xticks(y, categories)
    plt.bar(y, params)
    plt.ylabel('% of all twits')
    plt.title(title)
    plt.show()


# оценка по времени, временные рамки - с 12:00 до 20:00
def time_mark(data):
    data = data.copy()
    data = _sentimenting_twits(data)[::-1]
    t1 = time(12)
    t2 = time(20, 10)
    wnd = 30
    hour = 12
    numb_twit = 0
    good = 0
    bad = 0
    neutral = 0
    arr_chart = list()
    # чарты для времени
    x, yg, yb, yn, y = list(), list(), list(), list(), list()
    with open('hours.txt', 'w', encoding='utf-8') as file:
        for i in range(len(data)):
            if t1 <= data[i][0] <= t2:
                tm = time(hour, wnd)
                if data[i][0] <= tm:
                    if data[i][1] == '0':
                        neutral += 1
                    elif data[i][1] == '1':
                        good += 1
                    else:
                        bad += 1
                    numb_twit += 1
                    continue
                if wnd == 50:
                    hour += 1
                    wnd = 0
                else:
                    wnd += 10
                try:
                    arr_chart.append((tm, numb_twit, round(good / numb_twit, 3),
                                      round(neutral / numb_twit, 3), round(bad / numb_twit, 3)))
                except ZeroDivisionError:
                    arr_chart.append((tm, 0, 0, 0, 0))
        for el in arr_chart:
            file.write('12:00-{} : {} {}/{}/{}\n'.format(el[0], el[1], el[2], el[3], el[4]))
            # график
            x.append(datetime.strptime(str(el[0]), "%H:%M:%S"))
            y.append(el[1])
            yg.append(el[2])
            yn.append(el[3])
            yb.append(el[4])
    x = np.array(x)
    y = np.array(y)
    yg = np.array(yg)
    yn = np.array(yn)
    yb = np.array(yb)

    fmt = dates.DateFormatter('%H:%M:%S')
    fig, (ax1, ax2) = plt.subplots(figsize=(13, 8), nrows=2, ncols=1)

    ax1.set_title('Distribution of tweets classes in time')
    ax1.plot(x, yg, "ro-", label='N_pos')
    ax1.plot(x, yn, "go:", label='N_0')
    ax1.plot(x, yb, "bo--", label='N_neg')
    ax1.grid()
    ax1.legend()
    ax1.set_ylabel('Fraction')
    ax2.bar(x, y, width=0.004, color='brown')
    ax2.grid()
    ax2.set_ylabel('Number of tweets')
    ax2.set_xlabel('Time window')
    ax1.xaxis.set_major_formatter(fmt)
    ax2.xaxis.set_major_formatter(fmt)
    fig.autofmt_xdate()
    plt.show()


def main():
    f = open('data.txt', encoding="utf-8")
    s = f.read()
    after_changes = clear_twitter(s)
    tokens = after_changes[0]
    # print(tokens)
    # print(len(tokens))
    f.close()
    frequency(tokens)
    twits_length(tokens)
    adj_rating()
    first_rule(tokens)
    second_rule(tokens)
    time_mark(tokens)


if __name__ == "__main__":
    morph = pymorphy2.MorphAnalyzer()
    main()
