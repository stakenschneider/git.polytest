#!/bin/bash

# Если пользователь не указал имя файла, то выходим с ошибкой

filename=$1;
if [ -z $filename ]; then
	exit 1
fi

# Получим индексный дескриптор указанного файла, если не получилось, то выходим с ошибкой

inode="$(ls -i $filename | awk '{print $1}')"

if [ -z $inode ]; then
	exit 1
fi

# Рекурсивно ищем все жесткие ссылки на индексный дескриптор в домашнем каталоге пользователя

ls $HOME -l -R -i | grep ^$inode

