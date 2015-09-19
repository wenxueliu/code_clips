$./echo

new client :127.0.0.1:37778
new client :127.0.0.1:37779
RECV<-(127.0.0.1,37779): http request

SEND->(127.0.0.1,37779): http request

$ ./client
socket_writeable
c->writen = 13, c->length=13
socket_readable
read-> http request
^C


