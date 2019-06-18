from bs4 import BeautifulSoup
import requests
import json
from string import Template
import smtplib
# import necessary packages
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
import schedule
import time


url = "https://baltimore.craigslist.org/search/sss?query=nintendo+switch+-looking&search_distance=80&postal=21230&min_price=100&max_price=230&condition=20&condition=30&condition=40&condition=50"
response = requests.get(url, timeout=5)
content = BeautifulSoup(response.content, "html.parser")

switchArr = []

for result in content.findAll('li', attrs={"class": "result-row"}):
    switch = {
        "title": result.find('a', attrs={"class": "result-title hdrlnk"}).text,
        "price": result.find('span', attrs={"class": "result-price"}).text,
        "page":  result.a['href']
    }

    print(switch)
    print('\n')

    switchArr.append(switch)

with open('switchesAvail.json', 'w') as outfile:
    json.dump(switchArr, outfile)

outfile.close()



def get_info(filename):
    
    titles = []
    prices = []
    pages = []

    with open('switchesAvail.json', 'r') as json_data:
        jsonData = json.load(json_data)

    for i in jsonData:
        if "switch" in i['title'].lower():
            titles.append(i['title'])
            prices.append(i['price'])
            pages.append(i['page'])
    
    return titles, prices, pages


# Here is the read template function
def read_template(filename):
    with open(filename, 'r', encoding='utf-8') as template_file:
        template_file_content = template_file.read()
    return Template(template_file_content)


def sendMail():

    # Fill in your emails and password here
    pwd = '...'
    emailFrom = '...'
    emailTo = '...'

    titles, prices, pages = get_info('switchesAvail.json')
    message_template = read_template('notif.txt')

    # Set up SMTP
    s = smtplib.SMTP(host='smtp.gmail.com', port=587)
    s.starttls()
    s.login(email, pwd)


    # Main function, sending email
    # For each contact, send the email:
    for title, price, page in zip(titles, prices, pages):
        msg = MIMEMultipart()       # create a message

        # add in the actual person name to the message template
        message = message_template.substitute(TITLE=title, PRICE=price, URL=page)

        print(message)

        # setup the parameters of the message
        msg['From']=emailFrom
        msg['To']=emailTo
        msg['Subject']="NINTENDO SWITCH"

        # add in the message body
        msg.attach(MIMEText(message, 'plain'))

        # send the message via the server set up earlier.
        s.send_message(msg)
        
        del msg

        # Terminate session
    s.quit()

    return


def main():

    schedule.every().day.at("12:30").do(sendMail)

    while True:
        schedule.run_pending()
        time.sleep(60) # wait one minute
        print('working')


if __name__ == '__main__':
    main()