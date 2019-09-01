from app import db
from flask_mongoengine import BaseQuerySet

class Node(db.Document):
    nodeID = db.StringField()
    name = db.StringField()
    location = db.StringField()
    numberOfDevices = db.IntField(default=0)
    meta = {'collection': 'node'}
	
class Device(db.Document):
    deviceID = db.StringField()
    name = db.StringField()
    nodeName = db.StringField()
    location = db.StringField()
    status = db.IntField(default=0)
    meta = {'collection': 'device'}
	
	
class Wifi(db.Document):
    ssid = db.StringField()
    password = db.StringField()

