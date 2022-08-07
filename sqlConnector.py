import mysql.connector
from datetime import date
import pandas as pd

today = date.today().strftime("%Y-%m-%d")

mydb = mysql.connector.connect(
    host ="localhost",
    user = "aul_iot",
    password = "aultugasakhir",
    database = "aul_iot"
)

def postData(nama, status, keterangan):
    mycursor = mydb.cursor()
    sql = "INSERT INTO data (Nama, Status, Keterangan) VALUES ('%s', '%s', '%s')" %(str(nama), str(status), str(keterangan))
    mycursor.execute(sql)
    mydb.commit()

def getTodayData():
    mycursor = mydb.cursor()
    sql = "SELECT * FROM data WHERE CAST(Tanggal AS DATE) = CAST('%s' AS DATE) ORDER BY ID" %(str(today))
    mycursor.execute(sql)
    return mycursor.fetchall()
