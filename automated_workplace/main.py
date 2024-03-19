import sys
import asyncio
import qasync

from MRIMAApp import MRIMAApp

if __name__ == '__main__':
	app = MRIMAApp(sys.argv)

	event_loop = qasync.QEventLoop(app)
	asyncio.set_event_loop(event_loop)

	app_close_event = asyncio.Event()
	app.aboutToQuit.connect(app_close_event.set)

	event_loop.run_until_complete(app_close_event.wait())
	event_loop.close()

	#debug
	from Profiler import Profiler
	print("Profiler stats:")
	Profiler.getInstance().print()

