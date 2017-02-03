import os
import webbrowser
import json
  
data1 = '''
<link href="./jquery/jquery-ui.css" rel="stylesheet" />
<script type="text/javascript" src="./jquery/jquery.js"></script>
<script type="text/javascript" src="./jquery/jquery-ui.js"></script>
<script type="text/javascript" src="./jquery/jquery.canvasjs.min.js"></script>
<script type="text/javascript">
window.onload = function () {
	//Better to construct options first and then pass it as a parameter
	var options1 = {
		title: {
			text: "Page Replacement algorithm comparision"
		},
                animationEnabled: true,
		data: [
		{
		type: "bar", //change it to line, area, bar, pie, etc
			dataPoints: [
'''
data2 = '''
    ]
		}
		]
  	};

	  $("#resizable").resizable({
		create: function (event, ui) {
			//Create chart.
			$("#chartContainer1").CanvasJSChart(options1);
		},
		resize: function (event, ui) {
			//Update chart size according to its container's size.
			$("#chartContainer1").CanvasJSChart().render();
		}
	  });

    }</script>
'''
def func2():
  global data1, data2
  LI = []
  replacementcounts = []
  url = os.path.abspath('outputmainprogram.json')
  with open(url) as data_file:
    data = json.load(data_file)
  for AData in data:
    LI.append(AData['algoname'])
    replacementcounts.append(AData['numberofstageswithswap'])
    myfunc(AData)
  
  
  M = open(os.path.abspath('./menu.html'), 'w')
  menudata = '''
       <!DOCTYPE html>
      <html>
      <head>
      '''
  menudata += data1
  for i in range(len(replacementcounts)):
    menudata += "{label: \""+data[i]['algoname']+"\", y: "+str(replacementcounts[i])+" },"
  menudata += data2
  menudata += '''
      </head>
      <body><ol>
      '''

  for item in LI:
    menudata += "<li><a href=\"./"+item+".html\">"+item+"</a></li>"
  menudata += '''
  </ol>
  <div id="resizable" style="height: 300px;border:1px solid gray;">
	<div id="chartContainer1" style="height: 100%; width: 100%;"></div>
  </body>
  </html>
  '''
  M.write(menudata)
  M.close()
  browser(M)

def myfunc(data):
  global totalstages
  #os.system(commandstring)
  listof = []
  totalstages =  data['numberofstages']
  f = open(os.path.abspath(''+data['algoname']+'.html'), 'w')
  datatowrite = '''
  <!DOCTYPE html>
<html>
<head>
<style> 
box {
    width: 100px;
    height: 100px;
    border: 1px solid blue;
    border-radius: 3px; 
    padding: 8px;
background-color: yellow;
margin: 5%;
}
div {
text-align: center;
  padding: 20px;
}
</style>
</head>
<body>
<a href="./menu.html">MENU</a>
  '''
  for i in range(data['numberofstages']):
    datatowrite += "<div>stage"+str(i+1)
    for j in data['allstages'][i]:
      if j > 0:
        datatowrite += "<box >"+str(j)+"</box>"
      else:
        datatowrite += "<box >"+'-'+"</box>"
    datatowrite += "</div>"  
  datatowrite += str(data['numberofstages']) + '''
  
  </body>
</html>

  '''
  f.write(datatowrite);
def browser(M):
  url = os.path.abspath(M.name)
  webbrowser.open('file://' + url, new=2)




#print "navigate to menu.html"
func2()

