import os
import json
import websocket
import thread
import time

from tsps import tsps_connection

# Web server + socket server stuff

import thread
import sys

from twisted.internet import reactor
from twisted.python import log
from twisted.web.server import Site
from twisted.web.static import File

from autobahn.websocket import WebSocketServerFactory, \
                               WebSocketServerProtocol, \
                               listenWS
# TSPS listener

class TSPSServerProtocol(WebSocketServerProtocol):
	def onOpen(self):
		self.factory.register(self)

	def onMessage(self, msg, binary):
		print msg

	def connectionLost(self, reason):
		WebSocketServerProtocol.connectionLost(self, reason)
		self.factory.unregister(self)

class tsps_client(tsps_connection):
	stringMessage 	= {'type':'string', 'data':''}
	intMessage	 	= {'type':'number', 'data':'0'}
	boolMessage	 	= {'type':'boolean', 'data':'0'}
	length = 0
	server = None

	def __init__(self, _server, address="localhost", port=7681):
		self.server = _server
		self.length = 0
		self.connect(address, port)

	def onPersonEntered(self, person):
		print "## PERSON ENTERED ##"
		self.length += 1
		self.stringMessage["data"]	= "personEntered"
		self.intMessage["data"] 	= self.length

		if ( len(self.people) > 1 ):
			self.boolMessage["data"] 	= '1'
		else:
			self.boolMessage["data"] 	= '0'

		for c in self.server.clients:
			c.sendMessage(str(json.dumps(self.stringMessage)))
			c.sendMessage(str(json.dumps(self.intMessage)))
			c.sendMessage(str(json.dumps(self.boolMessage)))

	def onPersonUpdated(self, person):
		# print "## PERSON UPDATED ##"
		pass

	def onPersonLeft(self, person):
		print "## PERSON LEFT ##"

		self.length -= 1
		self.stringMessage["data"]	= "personLeft"
		self.intMessage["data"] 	= str(self.length)

		if ( len(self.people) > 1 ):
			self.boolMessage["data"] 	= '1'
		else:
			self.boolMessage["data"] 	= '0'

		# sMsg = json.dumps(self.stringMessage)
		# print sMsg
		# iMsg = json.dumps(self.intMessage)
		# bMsg = json.dumps(self.boolMessage)

		print json.dumps(self.stringMessage)

		for c in self.server.clients:
			c.sendMessage(str(json.dumps(self.stringMessage)))
			c.sendMessage(str(json.dumps(self.intMessage)))
			c.sendMessage(str(json.dumps(self.boolMessage)))

class BroadcastServerFactory(WebSocketServerFactory):
	def __init__(self, url, debug = False, debugCodePaths = False):
		WebSocketServerFactory.__init__(self, url, debug = debug, debugCodePaths = debugCodePaths)
		self.clients = []
		self.tickcount = 0

	def register(self, client):
		if not client in self.clients:
			print "registered client " + client.peerstr
         	self.clients.append(client)

	def unregister(self, client):
		if client in self.clients:
			print "unregistered client " + client.peerstr
			self.clients.remove(client)

	def broadcast(self, msg):
		print "broadcasting message '%s' .." % msg
		for c in self.clients:
			c.sendMessage(msg)
			print "message sent to " + c.peerstr

class tsps_sign_client(tsps_connection):
	stringMessage 	= {'type':'string', 'data':''}
	intMessage	 	= {'type':'number', 'data':'0'}
	boolMessage	 	= {'type':'boolean', 'data':'0'}
	length = 0
	client = None
	meeting = False

	def __init__(self, _client, address="localhost", port=7681):
		self.client = _client
		self.length = 0
		# Connect to TSPS
		self.connect(address, port)

	def onPersonEntered(self, person):
		print "## PERSON ENTERED ##"
		self.length += 1

		if len(self.people) > 1:
			if self.meeting is False:
				self.meeting = True
				self.stringMessage["data"]	= "SHH! MEETING! "
				self.client.send(str(json.dumps(self.stringMessage)))

	def onPersonUpdated(self, person):
		# print "## PERSON UPDATED ##"
		pass

	def onPersonLeft(self, person):
		print "## PERSON LEFT ##"

		self.length -= 1
		self.stringMessage["data"]	= "personLeft"
		self.intMessage["data"] 	= str(self.length)

		if len(self.people) <= 1 and self.meeting is True:
			self.meeting = False
			self.stringMessage["data"]	= "All clear, get loud! "
			self.client.send(str(json.dumps(self.stringMessage)))

# Main

if __name__ == '__main__':   
    ## GENERIC SENDER
 	# factory = BroadcastServerFactory("ws://localhost:9000")
	# factory.protocol = TSPSServerProtocol
	# factory.setProtocolOptions(allowHixie76 = True)	
	# listenWS(factory)
	# encoding = sys.getfilesystemencoding()
	# d = os.path.dirname(unicode(sys.executable, encoding))
	# webdir = File(d + "/../Resources/web")
	# web = Site(webdir)
	# reactor.listenTCP(8282, web)
	# thread.start_new_thread(tsps_client,(factory,"LAB-mini-06.local"))
	# reactor.run()

	## CUSTOM SENDER FOR SIGN
	# lab-mini-01.local:9094

	ws = websocket.WebSocketApp("ws://lab-mini-01.local:9094")
	thread.start_new_thread(tsps_sign_client,(ws,"LAB-mini-06.local"))
	ws.run_forever()
	
