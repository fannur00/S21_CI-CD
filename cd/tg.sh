#!/bin/bash

BOT_TOKEN="6929343647:AAHtAb_LuLVfmMOtGLqpEEBEBAFNn3ER1h8"

CHAT_ID="687268363"

# URL="https://api.telegram.org/bot${BOT_TOKEN}/sendMessage"

if [ "$CI_JOB_STATUS" == "success" ]; then
  MESSAGE="$CI_JOB_STAGE ✅"
else
  MESSAGE="$CI_JOB_STAGE 🚫"
fi

curl -s -X POST https://api.telegram.org/bot${BOT_TOKEN}/sendMessage -d chat_id=${CHAT_ID} -d text="${MESSAGE}" -d parse_mode="html"