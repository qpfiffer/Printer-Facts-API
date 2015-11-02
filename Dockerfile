FROM phusion/baseimage

RUN apt-get update && apt-get install build-essential git -y

RUN mkdir -p /app
RUN git clone https://github.com/qpfiffer/38-Moths.git /app/38-Moths; cd /app/38-Moths; make; sudo make install
COPY ./all_facts_simple.txt /app/all_facts_simple.txt
COPY main.c /app/main.c
COPY Makefile /app/Makefile

RUN cd /app && make

EXPOSE 8666
CMD ["./printer-facts"]
