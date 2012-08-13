import json
import websocket
import thread
import time

class tsps_connection(object):
	people = {}

	def connect(self, address="localhost", port="7681"):
		websocket.enableTrace(False)
		wsstring = "ws://" + address +":"+ str(port)

		self.ws = websocket.WebSocketApp(wsstring,
	                                on_message = self.on_message,
	                                on_error = self.on_error,
	                                on_close = self.on_close)
		self.ws.on_open = self.on_open
		self.ws.run_forever()

	def on_message(self, ws, message):
		obj = json.loads(message)

		if obj["type"] == "personEntered":
			self._onPersonEntered(obj);
		elif obj["type"] == "personMoved":
			self._onPersonMoved(obj);
		elif obj["type"] == "personUpdated":
			self._onPersonUpdated(obj);
		elif obj["type"] == "personWillLeave":
			self._onPersonLeft(obj);
		else:
			print "what the what " + obj["type"]

	def on_error(self, ws, error):
		print error

	def on_close(self, ws):
		print "### closed ###"

	def on_open(self, ws):
		print "### open ###"

	def _onPersonEntered(self, obj):
		if obj["id"] not in self.people:
			self.people[obj["id"]] = obj
			self.onPersonEntered(obj)

	def onPersonEntered(self, obj):
		pass

	def _onPersonUpdated(self, obj):
		if obj["id"] in self.people:
			self.people[obj["id"]] = obj
			self.onPersonUpdated(obj)
		else: 
			self._onPersonEntered(obj)

	def onPersonUpdated(self, obj):
		pass

	def _onPersonLeft(self, obj):
		try:
			del self.people[obj["id"]]	
		except:
			pass
		self.onPersonLeft(obj)	

	def onPersonLeft(self, obj):
		pass

class person(object):
	def __init__(self):
		self.id 			= -1;
		self.age 			= 0;

		self.boundingrect 	= {};
		self.boundingrect["x"] 		= 0;
		self.boundingrect["y"] 		= 0; 
		self.boundingrect["width"] 	= 0; 
		self.boundingrect["height"] = 0;

		self.height 	  	= 0;
		self.width 		 	= 0;
		self.x 				= 0;
		self.y 				= 0;
		self.centroid 		= {};
		self.centroid["x"] 	= 0;
		self.centroid["y"]  = 0;

		self.contours 		= [];
		self.depth 			= 0;
		self.haarrect 		= {};
		self.haarrect["x"] 	= 0;
		self.haarrect["y"]	= 0;
		self.haarrect["width"] = 0;
		self.haarrect["height"] = 0;

		self.opticalflow 	= {};
		self.opticalflow["x"] = 0;
		self.opticalflow["y"] = 0;

		self.velocity		= {};
		self.velocity["x"] 	= 0;
		self.velocity["y"] 	= 0;

	def update(self, message):
		pass