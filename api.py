from flask import Flask
from flask_restful import Api, Resource, reqparse
import pandas as pd
import sqlConnector

app = Flask(__name__)

app.config["DEBUG"] = True

api = Api(app)

parser = reqparse.RequestParser()

new_data = { 'nama':'', 'status':'', 'keterangan':''}

class aul(Resource):
    def get(self):
        data = sqlConnector.getTodayData()
        data = pd.DataFrame(data, columns=["ID","Tanggal","Nama","Status","Keterangan"])
        data["Tanggal"] = data["Tanggal"].dt.strftime("%Y-%m-%d %H:%M:%S")
        data = data.to_dict('records')
        return {'data' : data}, 200

    def post(self):
        parser.add_argument('nama', required=True, type=str)
        parser.add_argument('status', required=False, type=str)
        parser.add_argument('keterangan', required=False, type=str)
        args = parser.parse_args()
        new_data['nama'] = args['nama']
        new_data['status'] = args['status']
        new_data['keterangan'] = args['keterangan']
        sqlConnector.postData(str(args['nama']), str(args['status']), str(args['keterangan']))
        return {'data' : new_data}, 201

api.add_resource(aul, '/input')

if __name__ == '__main__':
    app.run(host="192.168.43.227",port=3306)