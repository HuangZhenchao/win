import os
import re
import csv
#
path='D:\Spider Resource\jilebox.com\\video\\good\\'
f=os.listdir(path)
# idlist=list()
# for name in f:
#     id=re.findall('([0-9]*).mp4',name)[0]
#     idlist.append(int(id))
#
# notlist=list()
# list=range(700000,701000)
# relist=list
# for i in list:
#     if i not in idlist:
#         notlist.append(i)
# print len(notlist)
# out = open('E:\Spider Resource\jilebox.com\\video\\notlist.csv', 'ab')
# csv_write = csv.writer(out, dialect='excel')
# for j in notlist:
#     stu=[j]
#     csv_write.writerow(stu)
# out.close()
#
import xlrd
workbook = xlrd.open_workbook(r'D:\\Spider Resource\\jilebox.com\\video\\jilevideo\\605988.xlsx')
sheet1 = workbook.sheet_by_index(0)
cols1 = sheet1.col_values(0)
#print cols1
cols2 = sheet1.col_values(1)

for filename in f:
    i = 0
    for name in cols2:
        if name==filename:
            newname=str(int(cols1[i]))+'.mp4'
            oldname=path+filename
            newname=path+newname
            print oldname
            print newname
            os.rename(oldname, newname)
            break
        i=i+1
