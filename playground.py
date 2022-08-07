from flask_restful import reqparse
import sqlConnector

parser = reqparse.RequestParser()
parser.add_argument('nama', required=True, type=str)
parser.add_argument('status', required=False, action='append', type=str)
parser.add_argument('keterangan', required=False, action='append', type=str)
args = parser.parse_args()
print(sqlConnector.postData(args['nama'], args['status'], args['keterangan']))