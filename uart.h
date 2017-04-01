

//initialize serial stuff
char UART_Init(const long int baudrate);

//write to TXREG
void UART_Write(char data, char dev);


//see status of TRMT bit: e.g, 
char trmtStatus();

char UART_Data_Ready();

char UART_Read(char dev);

void appendChar(char data);

char isItLongEnough();

void clearBuffer();

void copyTmpBuffer();

void writeString(char str[], char dev);

void writeResult(char dev);

char isEmpty();
