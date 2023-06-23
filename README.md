## What is
pico http server

## How to set up
1. Compile c file

```
$(~/pico-http-server) make http-server
```

2. Execute c file

```
$(~/pico-http-server) ./main.out -h (IP address) -p (port) --root (root directory)
```
※ Optional arguments need not be specified. In that case, IP address is `127.0.0.1`, port is `8080`, and root directory is `./`

## How to use
1. Send HEAD request to normal page
```
$ curl -I localhost:8080/new-permanent-index.html

HTTP/1.1 200 OK
Content-Type: text/html; charset=UTF-8
Content-Length: 826
```

2. Send GET request to normal page
```
$ curl -i localhost:8080/new-permanent-index.html

HTTP/1.1 200 OK
Content-Type: text/html; charset=UTF-8
Content-Length: 826

<!DOCTYPE html>
<html>
<head>
    <link rel="stylesheet" href="index.css">
    <title>自己紹介</title>
</head>
<body>
    <h1 class="title">ようこそ、新しいpermanent自己紹介ページへ</h1>
    <div class="container">
        <div class="text">
            <h1>名前</h1>
            <p>内藤 翔太</p>
            <h1>年齢</h1>
            <p>24</p>
            <h1>経歴</h1>
            <p>2017年 私立城北高等学校 卒業</p>
            <p>2021年 慶應義塾大学理工学部 卒業</p>
            <p>2023年 慶應義塾大学大学院理工学研究科 卒業</p>
            <h1>連絡先</h1>
            <p><a href="mailto:shota.naito@leverages.jp">shota.naito@leverages.jp</a></p>
        </div>
        <img src="shota.png" alt="自己紹介の画像">
    </div>
</body>
</html>
```

3. Send HEAD request to redirect page
```
$ curl -I localhost:8080/old-permanent-index.html

HTTP/1.1 308 Permanent Redirect
Content-Type: text/html; charset=UTF-8
Location: /new-permanent-index.html
Content-Length: 0
```

4. Send image POST request

```
$ curl -X POST --data-binary @shota.png http://localhost:8080/post-shota.png

<!DOCTYPE html><html><body><h1>201 Created</h1></body></html>
```
