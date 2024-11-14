#!/bin/bash

HOST="10.100.0.11"
USER="s23"
TMP_DIR="/home/s23"
DEST_DIR="/usr/local/bin"


scp -r src/cat $USER@$HOST:$TMP_DIR
scp -r src/grep/ $USER@$HOST:$TMP_DIR

if [ $? -eq 0 ]; then
    CI_DEPLOY_STATUS="success"
    echo "Артефакты успешно скопированы"
else
    CI_DEPLOY_STATUS="failed"
    echo "Ошибка при копировании артефактов"
    exit 1
fi


ssh $USER@$HOST "
				  rm -rf /usr/local/bin/cat; 
				  rm -rf /usr/local/bin/grep;
				  mv $TMP_DIR/cat $DEST_DIR; 
				  mv $TMP_DIR/grep $DEST_DIR
				" 

if [ $? -eq 0 ]; then
    CI_DEPLOY_STATUS="success"
    echo "Файлы успешно перемещены."
else
    CI_DEPLOY_STATUS="failed"
    echo "Ошибка при перемещении."
    exit 1
fi

