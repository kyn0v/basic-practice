""" lesson 1 - lesson 3: get + post + headers"""
# from urllib import request, parse
# import urllib
# import socket

# url = 'http://www.baidu.com'
# response = request.urlopen(url, timeout=1)
# print(response.read().decode('utf-8'))

# response = request.urlopen('http://httpbin.org/get', timeout=0.1)
# print(response.read())

# data = bytes(parse.urlencode({'word': 'hello world'}), encoding='utf8')
# print(data)
# try:
#     response2 = request.urlopen('http://httpbin.org/get', timeout=1)
#     print(response2.read())
# except urllib.error.URLError as e:
#     if isinstance(e.reason, socket.timeout):
#         print('TIME OUT!')

# url = 'http://httpbin.org/post'
# headers = {
#     "Accept":
#     "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9",
#     "Accept-Encoding":
#     "gzip, deflate",
#     "Accept-Language":
#     "zh-CN,zh;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6",
#     "Host":
#     "httpbin.org",
#     "Upgrade-Insecure-Requests":
#     "1",
#     "User-Agent":
#     "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.96 Safari/537.36 Edg/88.0.705.56",
# }
# dict = {'name': 'value'}
# data = bytes(parse.urlencode(dict), encoding='utf8')
# req = request.Request(url=url, data=data, headers=headers, method='POST')
# response = request.urlopen(req)
# print(response.read().decode('utf-8'))

""" lesson 4: requests"""
# import requests

# url = 'http://httpbin.org/get'
# data = {'key': 'value', 'abc': '456'}
# response = requests.get(url, data)
# print(response.text)

# url = 'http://httpbin.org/post'
# data = {'key': 'value', 'abc': '456'}
# response = requests.post(url, data)
# print(response.json())

""" lesson 5: regular expression"""
# import requests
# import re

# content = requests.get('http://www.cnu.cc/discoveryPage/hot-人像').text
# # print(content)
# # <a href="(.*?)".*?<div class="title">.*?</div>
# pattern = re.compile(r'<a href="(.*?)".*?<div class="title">(.*?)</div>', re.S)
# results = re.findall(pattern, content)
# # print(results)
# for result in results:
#     url, name = result
#     print(url, re.sub('\s', '', name))

""" lesson 6: beautifulsoup """
# import requests
# from bs4 import BeautifulSoup

# html_text = requests.get('http://www.cnu.cc/discoveryPage/hot-人像').text
# soup = BeautifulSoup(html_text, 'lxml')
# # print(soup.prettify)
# # print(soup.title.string)
# # print(soup.a)
# # print(soup.find_all('a'))
# # print(soup.find(id='link3'))

""" lesson 7: Scraping news title """
# from bs4 import BeautifulSoup
# import requests

# headers = {
#     "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9",
#     "Accept-Encoding": "gzip, deflate",
#     "Accept-Language": "zh-CN,zh;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6",
#     "connection": "close",
#     "Referer": "https://www.infoq.com",
#     "Upgrade-Insecure-Requests": "1",
#     "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.96 Safari/537.36 Edg/88.0.705.56",
# }

# def crawlNewsTitle(url):
#     response = requests.get(url, headers=headers)
#     soup = BeautifulSoup(response.text, 'lxml')
#     for title_href in soup.find_all('div', class_='card__data'):
#         print([title.get('title') for title in title_href.find_all('a') if title.get('title')])

# # page turing (single thread version)
# for i in range(15, 46, 15):
#     url = 'https://www.infoq.com/news/'+str(i)
#     crawlNewsTitle(url)

""" lesson 8: scraping pictures(singe thread version) """
import os
import shutil
from bs4 import BeautifulSoup
import requests
from requests.models import Response

headers = {
    "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9",
    "Accept-Encoding": "gzip, deflate",
    "Accept-Language": "zh-CN,zh;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6",
    "connection": "close",
    "Referer": "https://www.infoq.com",
    "Upgrade-Insecure-Requests": "1",
    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.96 Safari/537.36 Edg/88.0.705.56",
}


def downloadImages(image_url, save_path):
    response = requests.get(image_url, stream=True)
    if response.status_code == 200: # Avoid program suspending
        with open(save_path, 'wb') as f:
            response.raw.deconde_content = True
            shutil.copyfileobj(response.raw, f)


def crawlImages(url):
    response = requests.get(url, headers=headers)
    soup = BeautifulSoup(response.text, 'lxml')
    for pic_href in soup.find_all('div', class_='card__content'):
        for pic in pic_href.find_all('img'):
            imgurl = pic.get('src')
            dir = os.path.abspath('.')
            filename = os.path.basename(imgurl)
            imgpath = os.path.join(dir, filename)
            print('downloading %s' % imgurl)
            downloadImages(imgurl, imgpath)


pageNum = 0
for i in range(12, 37, 12):
    pageNum += 1
    print('page %d' % pageNum)
    url = 'https://www.infoq.com/presentations/'+str(i)
    crawlImages(url)
