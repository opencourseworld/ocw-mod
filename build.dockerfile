FROM devkitpro/devkita64

RUN apt update && apt install -y python3-pip
RUN pip3 install SarcLib==0.3 libyaz0==0.5 PyYAML==6.0.2 ipdb==0.13.13
COPY ./vendor/pymsb /tmp/pymsb
RUN cd /tmp/pymsb && pip3 install .
