
import os
import datetime
import time
from twitter import *
import sys
import sqlite3


def insertdb(tablename, currtuple, fields):
	conn = sqlite3.connect('tweets.db')
	mysql = ""
 	mytuple = str(tuple(currtuple)).replace("u'","'") 
	myfields = str(tuple(fields)).replace("u'","'") 	
	if fields == None: 
		mysql = "INSERT INTO %s VALUES %s" % (tablename,mytuple)
	else:
		mysql = "INSERT INTO %s %s VALUES %s" % (tablename,myfields,mytuple)

#	print mysql
	try:
		conn.execute(mysql)
	except sqlite3.OperationalError:
		print "operational:%s" % (mysql)
	except sqlite3.IntegrityError:
		print "repeated key: %s " % (mysql)

	conn.commit()

def deletedb(tablename):
	conn = sqlite3.connect('tweets.db')
 #	mysql =  "DELETE FROM %s where id>=0" % (tablename)
	mysql =  "DELETE FROM %s" % (tablename)

	print mysql	

	conn.execute(mysql)
	conn.commit()
	print "deleted all records"




def dateToUTC(currdate):
	mydate = currdate.replace(" +0000","")
	date_object = datetime.datetime.strptime(mydate, '%a %b %d %H:%M:%S %Y')
	return int(time.mktime(date_object.timetuple()))


mysearch = "victorrbravo"
mycount = 20
if len(sys.argv) > 1 and sys.argv[1]=="deleteall":
	deletedb("tweets")
	deletedb("users")
	exit()

if len(sys.argv) > 1:
	mysearch = sys.argv[1]
if len(sys.argv) > 2:
	mycount = int(sys.argv[2])



CONSUMER_KEY = "S5R5rpzDW1rWujNZy7owA"
CONSUMER_SECRET = "csxwzsKnEEdszFxUIPltprgfzIC70IzrWZni8m7AX4"

MY_TWITTER_CREDS = os.path.expanduser('~/.my_app_credentials')
if not os.path.exists(MY_TWITTER_CREDS):
    oauth_dance("My App Name", CONSUMER_KEY, CONSUMER_SECRET,
                MY_TWITTER_CREDS)

oauth_token, oauth_secret = read_token_file(MY_TWITTER_CREDS)

twitter = Twitter(auth=OAuth(
    oauth_token, oauth_secret, CONSUMER_KEY, CONSUMER_SECRET))

# Now work with Twitter
#twitter.statuses.update(status='Prueba!')
myobjs = twitter.search.tweets(q=mysearch,count=mycount)

items = myobjs.items()
mycount = 0

i = 0
for item in myobjs.items():
	if item[0] == "statuses":
		mycount = len(item[1])
		print "mycount %d" % (mycount) 
		for tw in item[1]:
			tfields = []
			ufields = []
			tcurrtuple = []
			ucurrtuple = []
			for key in tw.keys():
				if key == "user":
				#	print "\n---name:%s" % (tw[key]["name"])
					tfields.append(key)
					tcurrtuple.append(tw[key]["screen_name"])
					ufields.append("screen_name")
					ucurrtuple.append(tw[key]["screen_name"])
					ufields.append("name")
					ucurrtuple.append(tw[key]["name"])
					ufields.append("id")
					ucurrtuple.append(i)
					i= i + 1
				#	print "lugar: %s" %( tw[key]["location"])
					ufields.append("location")
					ucurrtuple.append(tw[key]["location"])
					tfields.append("location")
					tcurrtuple.append(tw[key]["location"])
	
				#	print "seguidores: %s" %( tw[key]["followers_count"])
					ufields.append("followers_count")
					ucurrtuple.append(tw[key]["followers_count"])
	
				#	print "Idioma: %s" %( tw[key]["lang"])
					ufields.append("lang")
					ucurrtuple.append(tw[key]["lang"])
					tfields.append("lang")
					tcurrtuple.append(tw[key]["lang"])
	
				#	print "Cuenta_status: %d" %( tw[key]["statuses_count"])
					ufields.append("statuses_count")
					ucurrtuple.append(tw[key]["statuses_count"])
	
				#	print "Amigos: %d" %( tw[key]["friends_count"])
					ufields.append("friends_count")
					ucurrtuple.append(tw[key]["friends_count"])
	
					mydate = tw[key]["created_at"] 
				#	print "--------Fecha_de_agregado: %s\n" % (mydate)
					utc_seconds = dateToUTC(mydate)
					ufields.append("created_at")
					ucurrtuple.append(utc_seconds)
	
				elif key == "text":
				#	print "*%s  ->" % (key)
				#	print tw[key]
					tfields.append(key)
					mytext = tw[key].replace("'","`")
					tcurrtuple.append(mytext)
				elif key == "created_at":			
					utc_seconds = dateToUTC(tw[key])
					tfields.append(key)
					tcurrtuple.append(utc_seconds)
				elif key == "id_str":
				#	print "*%s  ->" % (key)
				 #	print tw[key]
					tfields.append(key)
					tcurrtuple.append(tw[key])
					tfields.append("id")
					tcurrtuple.append(int(tw[key]))
			
			insertdb("tweets",tcurrtuple,tfields)			
			insertdb("users",ucurrtuple,ufields)			
		
		print "------------------------- "
		print " "
		


print "Cuenta de tweets: %d " % (mycount)


