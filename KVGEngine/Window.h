#pragma once
#include "pch.h"

namespace KGV::System 
{
	class Window 
	{
	public:
		/**
		 * @brief Creates the window with the set parameters
		 * @param log The logging Utility 
		 * @param width [OPTIONAL] Sets the desired width in pixels of the window. Default = 800
		 * @param height [OPTIONAL] Sets the desired height in pixels of the window. Default = 600
		 * @param xPos [OPTIONAL] Sets the desired x position of the window in pixels of the window. Default = 0 (Left)
		 * @param yPos [OPTIONAL] Sets the desired y position of the window in pixels of the window. Default = 0 (Top)
		 * @param caption [OPTIONAL] Sets the desired text (caption) at the top of the window. Default = "Hello World"
		 * @param showState [OPTIONAL] Sets the window as visible if true and hidden if false.
		*/
		explicit Window( S32 width = 800, S32 height = 600,
				S32 xPos = 0, S32 yPos = 0, const std::string& caption = "Hello World",
				bool showWindow = false);

		/**
		 * @brief Takes care of cleaning up the window.
		*/
		virtual	~Window();

		/**
		 * @brief Checks if the window is open on the desktop. 
		 * @return Returns false if the window is programmatically hidden, returns true if the window is
		 * open on the desktop and not if its visible by the user..
		*/
		virtual bool		isWindowVisible() = 0;

		/**
		 * @brief Changes the visibility of the window.
		 * @param showState True = Visibile, False = non visable.
		*/
		virtual void		showWindow( bool showState ) = 0;

		/**
		 * @brief Resizes the window with the desired dimensions.
		 * @param width Width of the window in pixels.
		 * @param height Height of the window in pixels.
		*/
		virtual void		resizeWindow( S32 width, S32 height ) = 0;

		/**
		 * @brief 
		 * @param xPos 
		 * @param yPos 
		*/
		virtual void		repositionWindow( S32 xPos, S32 yPos ) = 0;

		/**
		* @brief Sets the caption used by the window.
		* @param caption reference to the desired string.
		*/
		virtual void		setWindowCaption( const std::string& caption ) = 0;

		/**
		 * @brief Gets the caption currently used by the window.
		 * @return std::string& caption of the window.
		*/
		const std::string&	getWindowCaption();

		/**
			* @brief Returns the current width of the window in pixels.
			* @return Width in pixels.
		*/
		S32	getWidth();

		/**
			* @brief Returns the current height of the window in pixels.
			* @return height in pixels.
		*/
		S32	getHeight();

		/**
			* @brief Returns the current x position in pixel coordinates from 0 (left) of the display.
			* @return Position in pixels.
		*/
		S32	getXPos();

		/**
			* @brief Returns the current y position in pixel coordinates from 0 (top) of the display.
			* @return Position in pixels.
		*/
		S32	getYPos();

	protected:
		S32 width;
		S32 height;
		S32 xPos;
		S32 yPos;
		std::string windowCaption;
		bool isVisible;
	};
}