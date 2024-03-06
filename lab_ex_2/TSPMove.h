#ifndef TSP_MOVE_H
#define TSP_MOVE_H

class TSPMove {
   private:
    int from;
    int to;

   public:
    TSPMove();

    int getFrom() const;

    int getTo() const;

    void setFrom(int from);

    void setTo(int to);
};

#endif