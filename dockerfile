FROM alpine
COPY bear.cpp .
RUN apk add gcc g++
RUN g++ -o bear bear.cpp
CMD ./bear 1 5 4
