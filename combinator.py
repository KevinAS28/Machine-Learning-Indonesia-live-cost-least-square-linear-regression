import time, copy
class linked_node:
	the_next = None
	data0 = 0
	the_max = 0
	initialized = False
	
	def __init__(self, the_max=10, data0=0, the_next=None):
		self.the_next = the_next
		self.data0 = data0
		self.the_max = the_max
		self.initialized = True
	def __add__(self, number):
		obj = copy.deepcopy(self)
		obj.data0+=number
		val = (obj.data0-(obj.data0%obj.the_max))/obj.the_max
		obj.data0 = ((number)%obj.the_max)
		if (val!=0):
			obj.the_next += val		
		return obj

class linked_list:
	data = None
	temp = 0
	def __init__(self, size, base_num=10):
		self.data = linked_node(base_num)
		current = self.data
		for i in range(size-1):
			current.the_next = linked_node(base_num)
			current = current.the_next
	def base_loop(self, function):
		current = self.data
		index = 0
		while (True):
			if (current==None):
				break
			if (current.initialized):
				res = function(self,current, index)
				if (res==False):
					break
				# if (res!=None):
				# 	current = res
			current = current.the_next
			index+=1
		
	def check_initialized(self):
		init = 0
		def ci0(self, current, i):
			nonlocal init
			init+=1
		self.base_loop(ci0)
		return init

	def get_data(self):
		to_return = []
		def gc0(self, current, i):
			nonlocal to_return
			to_return.append(current.data0)
		self.base_loop(gc0)
		return to_return

	def __getitem__(self, index):
		obj = None
		def gi0(self, current, i):
			nonlocal obj
			if (i==index):
				obj = current
				return False
		self.base_loop(gi0)
		return obj

	def __setitem__(self, index, obj):
		if (index==0):
			self.data = obj

		current = self.data
		i = 0
		while (True):
			if (current==None):
				break
			if (i==index-1):
				current.the_next = obj
				break
			current = current.the_next
			i+=1

def main():
	for i in range(100):
		LL = linked_list(5)
		LL[0] += i
		print(i, LL.get_data())	
		time.sleep(0.3)
	
if __name__=="__main__":
	main()