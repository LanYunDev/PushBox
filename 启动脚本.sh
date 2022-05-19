#!/bin/bash

# Author: LanYun
# Blog: https://lanyundev.vercel.app
# Email: zestlanyun@gmail.com

# 建议不要修改文件夹名,👇下面这段代码用于判断本脚本是否处于当前项目的根目录
if [[ "`basename "$PWD"`" != "PushBox" ]]; then 
	echo "请确保本脚本位于本项目根目录处运行以便程序能够正常运行"
	echo "本程序当前运行目录:$PWD"
	echo "若你只修改了文件夹📁名,那么请按任意键继续."
	read -s -n 1 -p "按任意键继续.按q退出.30秒超时自动继续." -t 30 input
	if [[ $input == "q" || $input == "Q" ]]; then
		exit 0
	fi
fi

# 检测ninja是否存在,若不存在就安装
if ! command -v ninja >/dev/null 2>&1; then
	if [[ "$(uname)" == "Darwin" || `uname -a` =~ "Darwin" ]]; then
		brew install ninja
	elif [[ (-f "/etc/redhat-release" && `cat /etc/redhat-release` =~ "CentOS") || `uname -a` =~ "CentOS" ]]; then
		if ! command -v re2c >/dev/null 2>&1; then
			yum -y install git automake gcc+ gcc-c++ libtool
			git clone https://github.com/skvadrik/re2c.git re2c && cd re2c
			mkdir -p m4 && ./autogen.sh && ./configure --prefix=/usr && make
			sudo make install
		fi
		git clone git://github.com/ninja-build/ninja.git && cd ninja
		./configure.py --bootstrap
		cp ninja /usr/bin/
	elif [[ "$(expr substr $(uname -s) 1 5)" == "Linux" || `uname -a` =~ "Linux" ]]; then
		sudo apt install ninja-build
	else 
		echo "未能识别你的OS,即将尝试编译安装."
		git clone https://github.com/skvadrik/re2c.git re2c && cd re2c
		mkdir -p m4 && ./autogen.sh && ./configure --prefix=/usr && make
		sudo make install
		git clone git://github.com/ninja-build/ninja.git && cd ninja
		./configure.py --bootstrap
		cp ninja /usr/bin/
	fi
fi

# 检测并判断是否需要安装ncurses.h库
if [[ "$(uname)" == "Darwin" || `uname -a` =~ "Darwin" ]]; then
	if [[ ! -d "/opt/homebrew/Cellar/ncurses" ]]; then
		brew install ncurses
	fi
elif [[ (-f "/etc/redhat-release" && `cat /etc/redhat-release` =~ "CentOS") || `uname -a` =~ "CentOS" ]]; then
	if [[ ! -d "/usr/share/doc/libncursesw5-dev" ]]; then
		sudo yum install libncurses5 libncursesw5 libncursesw5-dbg
		sudo yum install libncursesw5-dev
	fi
elif [[ "$(expr substr $(uname -s) 1 5)" == "Linux" || `uname -a` =~ "Linux" ]]; then
	# if [[ ! -d "/usr/share/doc/libncurses5-dev" ]]; then
	# 	sudo apt-get install libncursesw5 libncursesw5-dev
	# 	sudo apt install libncurses5-dev
	# fi
	if [[ ! -d "/usr/share/doc/libncursesw5-dev" ]]; then
		sudo apt-get install libncursesw5-dev
	fi
elif [[ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]]; then
	echo "疑似windows用户?若是,非常抱歉本程序并不支持Win用户,请自行退出."
	echo "误报了?请反馈给我相关信息,本程序等待⌛️10秒后会继续运行"
	sleep 10
else 
	echo "未能识别你的OS,即将尝试编译安装."
	cd include/ncurses-6.3 && ./configure && make && sudo make install
fi

if [[ ! -d "build" ]]; then
	cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
	cmake --build build --target all
	mv build/PushBox src/
	cd src/ && ./PushBox
elif [[ -f "src/PushBox" ]]; then
	cd src/ && ./PushBox
fi
