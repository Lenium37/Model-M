class Line {
 public:
 	Line(int x0, int y0, int x1, int y1, int index) {
 		this->m_x0 = x0;
 		this->m_y0 = y0;
 		this->m_x1 = x1;
 		this->m_y1 = y1;
 		this->m_index = index;
 	};
 	Line() {};
 	~Line() {};

 	int get_x0() {
 		return this->m_x0;
 	};

 	int get_y0() {
 		return this->m_y0;
 	};

 	int get_x1() {
 		return this->m_x1;
 	};

 	int get_y1() {
 		return this->m_y1;
 	};

 	int get_index() {
 		return this->m_index;
 	};

 private:
 	int m_x0;
 	int m_y0;
 	int m_x1;
 	int m_y1;
 	int m_index;
};