#!/bin/bash

# Author: LanYun
# Blog: https://lanyundev.vercel.app
# Email: zestlanyun@gmail.com

# 注由于会使用github仓库的clone,如果感觉很慢,所以请使用科学上网.
# 已知问题：当第一次输入q后，后面的虽然有更新提示，依然不会更新。因为都用了同一个变量input

## 建议不要修改文件夹名,👇下面这段代码用于判断本脚本是否处于当前项目的根目录
#Catalog=$PWD
#if [[ "`basename "$Catalog"`" != "PushBox" ]]; then
#	echo "请确保本脚本位于本项目根目录处运行以便程序能够正常运行"
#	echo "本程序当前运行目录:$PWD"
#	echo "若你只修改了文件夹📁名,那么请按任意键继续."
#	read -s -n 1 -p "按任意键继续.按q键退出.30秒超时自动继续." -t 30 input
#	if [[ $input == "q" || $input == "Q" ]]; then
#		exit 0
#	fi
#fi

if [[ ! -d "build" ]]; then
#判断是否存在build文件夹

Catalog=$(dirname $(readlink -f "$0"))
# 检测CMake是否存在,若不存在就安装
if ! command -v cmake >/dev/null 2>&1; then
	echo "发现你的系统环境未安装CMake,即将尝试自动安装."
	if [[ "$(uname)" == "Darwin" || `uname -a` =~ "Darwin" ]]; then
		echo "为避免您的系统环境过旧,即将尝试更新你的系统环境."
		read -s -n 1 -p "按q键取消.否则3秒后自动开始." -t 3 input
		if [[ $input == "q" || $input == "Q" ]]; then
			echo "取消成功!"
			input=""
		else brew update && brew upgrade && brew cleanup
		fi
		brew install cmake
	elif [[ `uname -a` =~ "Manjaro" ]]; then
		echo "为避免您的系统环境过旧,即将尝试更新你的系统环境."
		read -s -n 1 -p "按q键取消.否则3秒后自动开始." -t 3 input
		if [[ $input == "q" || $input == "Q" ]]; then
			echo "取消成功!"
			input=""
		else sudo pacman -Syu
		fi
		sudo pacman -S cmake
	elif [[ (-f "/etc/redhat-release" && `cat /etc/redhat-release` =~ "CentOS") || `uname -a` =~ "CentOS" ]]; then
		sudo yum install cmake
	elif [[ "$(expr substr $(uname -s) 1 5)" == "Linux" || `uname -a` =~ "Linux" ]]; then
		echo "为避免您的系统环境过旧,即将尝试更新你的系统环境."
		read -s -n 1 -p "按q键取消.否则3秒后自动开始." -t 3 input
		if [[ $input == "q" || $input == "Q" ]]; then
			echo "取消成功!"
			input=""
		else
			sudo apt update && apt full-upgrade
			sudo apt-get update && apt-get upgrade && apt autoremove
		fi
		sudo apt-get install build-essential
		sudo apt install cmake
	else 
		echo "未能识别你的OS,即将尝试编译安装最新版."
		git clone https://github.com/Kitware/CMake.git
		cd CMake
		./bootstrap && make && sudo make install
		# if [[ `arch` =~ "aarch64" ]]; then
		# fi
	fi
fi

# 检测ninja是否存在,若不存在就安装
if ! command -v ninja >/dev/null 2>&1; then
	if [[ "$(uname)" == "Darwin" || `uname -a` =~ "Darwin" ]]; then
		echo "为避免您的系统环境过旧,即将尝试更新你的系统环境."
		read -s -n 1 -p "按q键取消.否则3秒后自动开始." -t 3 input
		if [[ $input == "q" || $input == "Q" ]]; then
			echo "取消成功!"
			input=""
		else brew update && brew upgrade && brew cleanup
		fi
		brew install ninja
	elif [[ `uname -a` =~ "Manjaro" ]]; then
		echo "为避免您的系统环境过旧,即将尝试更新你的系统环境."
		read -s -n 1 -p "按q键取消.否则3秒后自动开始." -t 3 input
		if [[ $input == "q" || $input == "Q" ]]; then
			echo "取消成功!"
			input=""
		else sudo pacman -Syu
		fi
		sudo pacman -S ninja
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
		echo "为避免您的系统环境过旧,即将尝试更新你的系统环境."
		read -s -n 1 -p "按q键取消.否则3秒后自动开始." -t 3 input
		if [[ $input == "q" || $input == "Q" ]]; then
			echo "取消成功!"
			input=""
		else
			sudo apt update && apt full-upgrade
			sudo apt-get update && apt-get upgrade && apt autoremove
		fi
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
		echo "为避免您的系统环境过旧,即将尝试更新你的系统环境."
		read -s -n 1 -p "按q键取消.否则3秒后自动开始." -t 3 input
		if [[ $input == "q" || $input == "Q" ]]; then
			echo "取消成功!"
			input=""
		else brew update && brew upgrade && brew cleanup
		fi
		brew install ncurses
	fi
elif [[ `uname -a` =~ "Manjaro" ]]; then
	echo "为避免您的系统环境过旧,即将尝试更新你的系统环境."
		read -s -n 1 -p "按q键取消.否则3秒后自动开始." -t 3 input
		if [[ $input == "q" || $input == "Q" ]]; then
			echo "取消成功!"
			input=""
		else sudo pacman -Syu
		fi
	sudo pacman -S ncurses
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
		echo "为避免您的系统环境过旧,即将尝试更新你的系统环境."
		read -s -n 1 -p "按q键取消.否则3秒后自动开始." -t 3 input
		if [[ $input == "q" || $input == "Q" ]]; then
			echo "取消成功!"
			input=""
		else
			sudo apt update && apt full-upgrade
			sudo apt-get update && apt-get upgrade && apt autoremove
		fi
		sudo apt-get install libncursesw5-dev
	fi
elif [[ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]]; then
	echo "疑似windows用户?若是,非常抱歉本程序并不支持Win用户,请自行退出."
	echo "误报了?请反馈给我相关信息,本程序等待⌛️10秒后会继续运行"
	sleep 10
else 
	echo "未能识别你的OS,即将尝试编译安装."
	cd $Catalog
	cd include/ncurses-6.3 && ./configure && make && sudo make install
fi

cd $Catalog
#if [[ ! -d "build" ]]; then
	cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
	cmake --build build --target all
	mv build/PushBox src/
	cd src/ && ./PushBox
elif [[ -f "src/PushBox" ]]; then
	cd src/ && ./PushBox
fi
