#!/usr/bin/env python
# -*- coding: utf-8 -*-

from sys import argv
from sys import stdin
from sys import exit
import smtplib
import email
from email.parser import Parser
from email.header import Header
from email.mime.text import MIMEText
from datetime import datetime, date, time
from collections import defaultdict
import random
import json


DEBUG = False

log_file = "/tmp/email" + str(datetime.now()) + ".log"

def print_log(message):
	with open(log_file, "a") as f:
		f.write(message)


def getEmails(orig):
	print_log("email: %s\n" % orig)
	if ">" in orig:
		orig = orig[orig.find("<") + 1:orig.find(">")]
	a = open("/var/lib/mailnof/cafe.txt","r")
	results = set()
	for i in a.readlines():
		results.add(i.strip().strip("\n"))
	results.add(orig)
	
	print_log("email: %s\n" % orig)
	
	a = open("/var/lib/mailnof/cafe.txt","w")
	for i in list(results):
		a.writelines(i.strip("\n") + "\n")
	return list(results)


msg = email.message_from_string(stdin.read())

# Evitar emails de fora do GTA para a lista de café
if '@gta.ufrj.br' not in msg['from']:
	exit(0)

destinatarios = getEmails(msg['from'])

placar = json.loads(open("/var/lib/mailnof/placar.json","r").read())


placar2 = defaultdict(int)
emails = {}

fromMSG=prev = msg['from']

if fromMSG in placar:
	placar[fromMSG] += 1
else:
	placar[fromMSG] = 1



for i in placar:
	nomeEmail = i
	valor =  placar[i]

	nome = nomeEmail.split("<")[0]
	
        if "<" in nomeEmail:
		email = nomeEmail.split("<")[1].strip(">")
	else:
		email = nomeEmail
	
	if nome != email:
		emails[email] = nome
	
	placar2[email] += valor

if 'debug' in argv:
	DEBUG = True
	destinatarios = list()

print_log("argc " + str(len(argv)) + "\n")
print_log("argv " + str(argv) + "\n")
print_log("destinatarios " + str(destinatarios)+ "\n")


print_log("EMAIL\n\n")

open("/var/lib/mailnof/placar.json","w").write(json.dumps(placar))

placarStr = "Placar:"
for i in placar2:
	placarStr = placarStr + "\n" + str(emails.get(i, i)) + ": " + str(placar2[i])


#for i in placar:
#	placarStr = placarStr + "\n" + str(i).split("<")[0].strip() + ": " + str(placar[i])


prev = msg['subject']
del msg['subject']

now = datetime.now()
msg['subject'] = Header("[Cafe] Saiu café às " + now.strftime("%Hh e %Mmin"), 'utf-8')

msg.add_header('Reply-To', fromMSG)
msg.add_header('sender', msg['to'])


msg.set_type("text/plain; charset=UTF-8")

del msg['to']


e = random.choice(placar2.keys())
nome = str(emails.get(e, e))

placarStr += "\n\n%s, o próximo é você! ;)\n\n" % (nome.strip())

oldmsg = msg.get_payload()

payload = "Saiu café fresquinho! \n\n" + placarStr

if not msg.is_multipart():
	payload += "\n\n" + str(oldmsg)

msg.set_payload(payload) # MIMEText(payload, 'plain', 'utf-8'))

print_log(msg.as_string())

s = smtplib.SMTP('localhost')
s.sendmail(msg['to'], destinatarios + [msg['from']], msg.as_string())

s.quit()

