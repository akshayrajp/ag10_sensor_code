import constants as C
import requests

def telegram_bot_sendtext(bot_message):

   bot_token = C.telegram_token
   bot_chatID = C.telegram_chatid 
   for chatid in bot_chatID:
       send_text = 'https://api.telegram.org/bot' + bot_token + '/sendMessage?chat_id=' + chatid + '&parse_mode=Markdown&text=' + bot_message
   response = requests.get(send_text)
