---
sidebar_position: 1
---


# Getting started



## C++ development

### One time setup

```bash


Run

```bash
./bootstrap_venv.sh
```

You'd expected to see something like this:

```bash
...
+ VENV_TAR_URL=https://github.com/farm-ng/farm-ng-core/releases/download/v.11e436d/venv-ubuntu-20.04.tar.gz
+ curl -L https://github.com/farm-ng/farm-ng-core/releases/download/v.11e436d/venv-ubuntu-20.04.tar.gz
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
  0     0    0     0    0     0      0      0 --:--:-- --:--:-- --:--:--     0
100 40.4M  100 40.4M    0     0   9.8M      0  0:00:04  0:00:04 --:--:-- 10.9M
+ rm -rf $HOME/farm-ng-core/venv/prefix
+ tar -xzf $HOME/code/farm-ng-core/scripts/venv.tar.gz --strip-components=1 -C $HOME/code/farm-ng-core/
+ echo 11e436d
+ echo '[Updated binary dependencies. Done!]'
[Updated binary dependencies. Done!]
```

### Source the environment

```bash
 source venv/bin/activate
 ```


## Docs

### One time setup (Ubuntu 20.04)



```bash
curl -sS https://dl.yarnpkg.com/debian/pubkey.gpg | sudo apt-key add -
echo "deb https://dl.yarnpkg.com/debian/ stable main" | sudo tee /etc/apt/sources.list.d/yarn.list
sudo apt update
sudo apt install yarn
```

Inside the doc folder:

```bash
sudo yarn install --frozen-lockfile
```

### Build docs locally

```bash
cd docs
yarn start
```
