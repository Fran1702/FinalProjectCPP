#include <iostream>
#include <queue>

using namespace std;

// print the Queue
//


class Message{
	public:
		enum class Type {TELEMETRY, ACTION};
		Type getType() const{return type;};
		void setType(Type t){type = t;};
		void send_msg(queue <Message>& q){q.push(*this);};
		// Le mando la cola a la cual debe ser enviado el msg
		void write_msg();
		//Message(queue <Message>& q_):q(q_){}; 
//		Message(); 
		void print(){cout << "Print";};
	private:
		Type type;
};
#define MSG_TELEMETRY Message::Type::TELEMETRY
#define MSG_ACTION Message::Type::ACTION

class TelemetryMsg: public Message{
	public:
		TelemetryMsg(){setType(MSG_TELEMETRY);};
		void write_msg(int x_){x = x_;};
	private:
		int x;	
};


void showq(queue <Message> gq){
	queue <Message> g = gq; // hago una copia de la queue
	while(!g.empty()){
		cout << '\t';
	       	g.front().print();
		g.pop();
	}
	cout << '\n' ;
}
int main(){

	queue <Message> qMSG; // Armo la cola de los mensajes
	TelemetryMsg m;
	TelemetryMsg m2;
	m.send_msg(qMSG);
	m2.send_msg(qMSG);

	if(m.getType() == MSG_TELEMETRY)
		cout << "ES TELEMETRICO PAPA" << endl;
	if(m2.getType() == MSG_TELEMETRY)
		cout << "ES TELEMETRICO PAPA" << endl;

	cout << "The queue gquiz is : ";
	showq(qMSG);
	return 0;
}
