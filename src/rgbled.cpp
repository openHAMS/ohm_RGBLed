#include "rgbled.hpp" //include the declaration for this class

class Line
{
	public:
	void setLength( double len );
	double getLength( void );
	Line();// This is the constructor declaration
	~Line();// This is the destructor: declaration

	private:
	double length;
};

// Member functions definitions including constructor
Line::Line(void)
{

}
Line::~Line(void)
{

}

void Line::setLength(double len)
{

}

double Line::getLength()
{

}
// Main function for the program
int main( )
{
	Line line;

	// set line length
	line.setLength(6.0);


	return 0;
}
