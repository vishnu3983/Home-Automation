from app import app

from models import *
from flask import request, render_template
from flask import jsonify, redirect, Response
import random
import datetime
import time
import jwt
from app import db


@app.route('/getDevices', methods=['POST', 'OPTIONS'])
def getDevicesMethod():
    content = request.get_json(force=True)
    nodeObject = Node.objects.get(nodeID=content['nodeID'])
    #nodeObject = Node.objects.get(nodeID='node01')
    numberOfDevics = nodeObject["numberOfDevices"]
    devices = [({"deviceID": device["deviceID"], "name": device["name"], "location":device["location"], "status": device["status"]}) for device in Device.objects(nodeName=nodeObject['name'])]
    print(devices)
    return jsonify(devices)

@app.route('/getNodes', methods=['GET', 'OPTIONS'])
def getNodesMethod():
    #content = request.get_json(force=True)
    #nodeObject = Node.objects.get(nodeID=content['nodeID'])
    nodes = [({"nodeID": node["nodeID"], "name": node["name"], "location":node["location"], "numberOfDevices": node["numberOfDevices"]}) for node in Node.objects()]
    print(nodes)
    return jsonify(nodes)

@app.route('/device/register', methods=['POST', 'OPTIONS'])
def registerDevice():
	deviceID = request.form['deviceID']
	name = request.form['name']
	nodeName = request.form['nodeName']
	if Node.objects(name=nodeName):
		node = Node.objects.get(name=nodeName)
		node.numberOfDevices+=1
		location = node.location+'/'+deviceID
		node.save()
		device = Device(name=name, deviceID=deviceID, nodeName=nodeName, location=location)
		device.save()
		return jsonify({"message": "Register success"})
		#return render_template('C:\\Users\\amd\\Desktop\\mqtt-app\\app.html')
		
	else:
		print("Node does not exist")
		return jsonify({"message": "Node does not exist"})
	
@app.route('/node/register', methods=['POST', 'OPTIONS'])
def nodeDevice():
	floor = request.form['floor']
	room = request.form['room']
	location = floor+'/'+room
	name = request.form['name']
	nodeID = request.form['nodeID']
	if Node.objects(nodeID=nodeID):
		print('Node already exists')
		return jsonify({"message": "Node does not exist"})
		
	else:
		node = Node(nodeID=nodeID,name=name,location=location)
		node.save()
		return jsonify({"message": "Node created"})
		#return render_template('C:\\Users\\amd\\Desktop\\mqtt-app\\app.html')
		 
	
@app.route('/turnOnDevice', methods=['GET', 'OPTIONS'])
def turnOnDeviceMethod():
    print("device turned on")
    return jsonify({"message": "device turned on"})

@app.route('/turnOffDevice', methods=['GET', 'OPTIONS'])
def turnOffDeviceMethod():
    print("device turned off")
    return jsonify({"message": "device turned off"})
