/*
Expressions calculation，such as 5 + 12 / （1+2）


接受一个正确的四则表达式作为输入，输出计算结果
example:
[input :] 5 + 12 / （1+2）
[output :] 9

计算思路：
申请一个数据栈 ，一个操作符栈
读取表达式：
	1遇到操作数，数据直接入栈
	2遇到操作符，若当前操作符比栈顶操作符优先级低，则从数据栈取操作数运算并将结果入栈，否则操作符直接入栈；
		表达式读取结束，以此从数据栈取出数据，操作符栈取操作符，并将结果入栈
		最终操作符栈为空，数据栈只有一个数，即为最终结果

*/

#include <iostream>
#include <stack>
#include <string> 
#include <cstring>
using namespace std;

class Exp{
	stack<double> ds;
	stack<char> ops;
	double rh,lh,v;
	char op;
	
public:
	Exp(){
		v = 0;
	}
	//读取并计算表达式 
	double calinput(){
		do{
			readdata();
			skipspace();
		} while(readop());
		calremain();
		return v;
	}
	
	void readdata(){
		while(!(cin >> v)){
			cin.clear();
			cin >> op;
			if(op!='(') throw string("在该遇到数值的地方遇到了") + op;
			ops.push(op); 
		}
		ds.push(v);
	}
	
	void skipspace(){
		while(cin.peek() == ' ') cin.ignore(); 
	}
	
	bool readop(){
		while((op=cin.get()) == ')'){
			while(ops.top() != '('){
				rh = ds.top();ds.pop();
				lh = ds.top();ds.pop();
				ds.push(cal(lh,ops.top(),rh));
				ops.pop();
			}
			ops.pop();
		}
		
		if(op=='\n') return false;
		if(strchr("+-*/",op) == NULL) //无效运算符
			throw string("无效运算符") + op;
			
		while(!ops.empty() && ops.top() != '(' && !prior(op,ops.top())){
			rh = ds.top();ds.pop();
			lh = ds.top();ds.pop();
			ds.push(cal(lh,ops.top(),rh));
			ops.pop();
		} 
		
		ops.push(op);
		return true;
	}
	
	void calremain(){
		while(!ops.empty()){
			rh = ds.top();ds.pop();
			lh = ds.top();ds.pop();
			ds.push(cal(lh,ops.top(),rh));
			ops.pop();
		}
		
		if(ds.size() != 1) throw string("无效的表达式");
		v = ds.top();
		ds.pop();
	} 
	
	double cal(double lh,char op,double rh){
		switch(op){
			case '+': return lh+rh;
			case '-': return lh-rh;
			case '*': return lh*rh;
			case '/': return lh/rh;
				
		}
	}
	
	bool prior(char o1,char o2){
		return o1!='+'&&o1!='-'&&o2!='*'&&o2!='/';
	}
};

int main(){
	
	Exp e;
	try{
		cout << e.calinput() << endl;
	}catch(const string& e ){
		cout << e << endl;
	}
	return 0;
}
 
