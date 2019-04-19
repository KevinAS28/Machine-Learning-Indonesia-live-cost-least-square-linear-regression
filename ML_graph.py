import matplotlib.pyplot as plt
import MlIdlcCore as ML
def main():

	ML.startThink()
	xdata = ML.getXData()
	ydata = ML.getYData()
	xtotal = ML.getXTotal()
	b = ML.getB()
	y = ML.getY()
	plt.title ("Prediction using least square regression\nX(Input) = %d\n Y(Your cost) = %d"%(xtotal, y));
	plt.scatter(xdata, ydata, color = "m", marker = "o", s = 15)
	x_show = [min(xdata), xtotal]
	y_show = [b, y]
	# new_x = np.array(list(m*i+b for i in x))
	plt.plot(x_show, y_show,color = "r", linewidth=3)
	plt.xlabel("The consume: food + water + electric + internet + clothes wash");
	plt.ylabel("Cost in rupiah / day");
	plt.show()

if __name__=="__main__":
	main()