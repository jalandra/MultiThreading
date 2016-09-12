class MMT implements Runnable{
	int i;
	mmt(){
		i = 1;
	}
	synchronized public printEvenNumber(){
		if(i>100)
			return;
		if(i%2==0){
			System.out.println(i);
			i++;
			notifyAll();
		} else {
			wait();
		}
		printEvenNumber()
	}
	synchronized public printOddNumber(){
		if(i>100)
			return;
		if(i%2!=0){
			System.out.println(i);
			i++;
			notifyAll();
		} else {
			wait();
		}
		printOddNumber()
	}
	
public static void main(){
	MMT mmt = new MMT()
	Thread t1 = new Thread(mmt){
		run(){
			mmt.printEvenNumber();
		}
	}
	Thread t2 = new Thread(mmt){
		run(){
			mmt.printOddNumber();
		}
	}
	try {
		t1.start();
	}catch(IllegalStateException e){
		System.out.println(e);
	}
	try {
		t2.start();
	}catch(IllegalStateException e){
		System.out.println(e);
	}
}

}
