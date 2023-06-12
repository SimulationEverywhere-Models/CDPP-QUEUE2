/*******************************************************************
*
*  DESCRIPTION: Atomic Model Queue
*
*  AUTHOR: Amir Barylko & Jorge Beyoglonian 
*
*  EMAIL: mailto://amir@dc.uba.ar
*         mailto://jbeyoglo@dc.uba.ar
*
*  DATE: 27/6/1998
*
*******************************************************************/

/** include files **/
#include "queue.h"      // class Queue
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )

/** public functions **/

/*******************************************************************
* Function Name: Queue
* Description: 
********************************************************************/
Queue::Queue( const string &name )
: Atomic( name )
, in( addInputPort( "in" ) )
, stop( addInputPort( "stop" ) )
, done( addInputPort( "done" ) )
, out( addOutputPort( "out" ) )
// , preparationTime( 0, 0, 10, 0 )

{
	string time( MainSimulator::Instance().getParameter( description(), "preparation" ) ) ;

	if( time != "" )
		preparationTime = time ;
			MainSimulator::Instance().outputStream() << "." << "\n";
			MainSimulator::Instance().outputStream() << "Preparation Time=" << preparationTime << "\n";
			MainSimulator::Instance().outputStream() << "." << "\n";
}

/*******************************************************************
* Function Name: initFunction
* Description: Resetea la lista
* Precondition: El tiempo del proximo evento interno es Infinito
********************************************************************/
Model &Queue::initFunction()
{
	elements.erase( elements.begin(), elements.end() ) ;
			MainSimulator::Instance().outputStream() << "." << "\n";
			MainSimulator::Instance().outputStream() << "INIT" << "\n";
			MainSimulator::Instance().outputStream() << "." << "\n";
	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description: 
********************************************************************/
Model &Queue::externalFunction( const ExternalMessage &msg )
{

/***** DEBUG  *****/
			MainSimulator::Instance().outputStream() << "." << "\n";
			MainSimulator::Instance().outputStream() << "Control EXTERNAL >" << "\n";
			MainSimulator::Instance().outputStream() << " msg.time()=" << msg.time() << "\n";
			MainSimulator::Instance().outputStream() << " msg.value()=" << msg.value() << "\n";
			MainSimulator::Instance().outputStream() << " lastChange()=" << lastChange() << "\n";
			MainSimulator::Instance().outputStream() << " nextChange()=" << nextChange() << "\n";
			MainSimulator::Instance().outputStream() << " state()=" << state() << "\n";
			MainSimulator::Instance().outputStream() << " elements.front()=" << elements.front() << "\n";
			MainSimulator::Instance().outputStream() << "< Control EXTERNAL" << "\n";
			MainSimulator::Instance().outputStream() << "." << "\n";
/***** DEBUG  *****/

	if( msg.port() == in )
	{
			MainSimulator::Instance().outputStream() << "." << "\n";
			MainSimulator::Instance().outputStream() << "EXT: IN" << "\n";
			MainSimulator::Instance().outputStream() << "." << "\n";
	
			elements.push_back( msg.value() ) ;
			MainSimulator::Instance().outputStream() << " elements.front()=" << elements.front() << "\n";
		
		if( elements.size() == 1 )
			MainSimulator::Instance().outputStream() << "." << "\n";
			MainSimulator::Instance().outputStream() << "holdin active: preparation time=" << preparationTime << "\n";
			MainSimulator::Instance().outputStream() << "." << "\n";

			holdIn( active, preparationTime );
	}

	if( msg.port() == done )
	{
			MainSimulator::Instance().outputStream() << "." << "\n";
			MainSimulator::Instance().outputStream() << "EXT: DONE" << "\n";
			MainSimulator::Instance().outputStream() << "." << "\n";
			elements.pop_front() ;
			MainSimulator::Instance().outputStream() << " elements.front()=" << elements.front() << "\n";

		if( !elements.empty() )
			MainSimulator::Instance().outputStream() << "." << "\n";
			MainSimulator::Instance().outputStream() << "holdin active: preparation time=" << preparationTime << "\n";
			MainSimulator::Instance().outputStream() << "." << "\n";

			holdIn( active, preparationTime );
	}

	if( msg.port() == stop )
	{
		    MainSimulator::Instance().outputStream() << "." << "\n";
			MainSimulator::Instance().outputStream() << "EXT: STOP/START" << "\n";
			MainSimulator::Instance().outputStream() << "." << "\n";
		if( state() == active && msg.value() == 1 )
		{
//			timeLeft = msg.time() - lastChange();
//			timeLeft = lastChange() + nextChange() - msg.time();
			timeLeft = nextChange();

			MainSimulator::Instance().outputStream() << "." << "\n";
			MainSimulator::Instance().outputStream() << "memorizo un timeLeft=" << timeLeft << "\n";
			MainSimulator::Instance().outputStream() << "." << "\n";

				passivate();
		} else
			 
				if( state() == passive && msg.value() == 0 )
			{
				MainSimulator::Instance().outputStream() << "." << "\n";
				MainSimulator::Instance().outputStream() << "me despertaron, me despierto durante un timeLeft=" << timeLeft << "\n";
				MainSimulator::Instance().outputStream() << "." << "\n";

				holdIn( active, timeLeft );
			}
			
	}
	return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description: 
********************************************************************/
Model &Queue::internalFunction( const InternalMessage & )
{

/***** DEBUG  *****/
			MainSimulator::Instance().outputStream() << "." << "\n";
			MainSimulator::Instance().outputStream() << "Control INTERNAL >" << "\n";
			MainSimulator::Instance().outputStream() << " lastChange()=" << lastChange() << "\n";
			MainSimulator::Instance().outputStream() << " nextChange()=" << nextChange() << "\n";
			MainSimulator::Instance().outputStream() << "< Control INTERNAL" << "\n";
			MainSimulator::Instance().outputStream() << "." << "\n";
/***** DEBUG  *****/

	passivate();
	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description: 
********************************************************************/
Model &Queue::outputFunction( const InternalMessage &msg )
{
			MainSimulator::Instance().outputStream() << "." << "\n";
	MainSimulator::Instance().outputStream() << "OUTPUT" << "\n";
			MainSimulator::Instance().outputStream() << "." << "\n";
	sendOutput( msg.time(), out, elements.front() ) ;
	return *this ;
}
