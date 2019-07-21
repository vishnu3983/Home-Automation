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
    nodeObject = Node.objects(nodeID=content['nodeID']).get()
    numberOfDevics = nodeObject["numberOfDevices"]
    devices = [({"id": device["deviceID"], "name": device["name"], "location":device["location"], "status": device["status"]}) for device in Device.objects(nodeID=content["nodeID"])]
    print(devices)
    return jsonify(devices)



@app.route('/turnOnDevice', methods=['GET', 'OPTIONS'])
def turnOnDeviceMethod():
    print("device turned on")
    return jsonify({"message": "device turned on"})

@app.route('/turnOffDevice', methods=['GET', 'OPTIONS'])
def turnOffDeviceMethod():
    print("device turned off")
    return jsonify({"message": "device turned off"})
