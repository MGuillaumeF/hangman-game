import React from "react";

/**
 * The min value to display hangman draw
 */
export const MIN_HANGMAN_VALUE = 0;
/**
 * The max value to display hangman draw
 */
export const MAX_HANGMAN_VALUE = 11;

type Props = {
  /**
   * The background color of drawing area
   */
  backgroundColor?: string;
  /**
   * The stroke color of parts of hangman draw
   */
  color?: string;
  /**
   * The height of Hangman draw
   */
  height: number;
  /**
   * The quantity of Hangman's parts to draw
   */
  quantity: number;
  /**
   * The width of Hangman draw
   */
  width: number;
};

/**
 * SVG Path Component Builder Wrapper
 * @param param0 @see SVGPathElement
 * @returns JSX.Element of SVG draw
 */
function Path({ children, ...props }: React.SVGProps<SVGPathElement>) {
  return <path {...props}>{children}</path>;
}
/**
 * SVG Circle Component Builder Wrapper
 * @param param0 @see SVGCircleElement
 * @returns JSX.Element of SVG draw
 */
function Circle({ children, ...props }: React.SVGProps<SVGCircleElement>) {
  return <circle {...props}>{children}</circle>;
}

interface IPartProps extends React.SVGProps<SVGElement> {
  /**
   * SVG Component Builder Wrapper
   * @param props The props to draw the part of hangman
   * @returns The JSX.Element of the part of hangman
   */
  Type: (props: React.SVGProps<any>) => JSX.Element;
}

const parts: IPartProps[] = [
  {
    /**
     * Bottom support
     */
    Type: Path,
    d: "M0 487.5 L125 487.5",
    strokeWidth: "25"
  },
  {
    /**
     * Column support
     */
    Type: Path,
    d: "M37.5 475 L37.5 10",
    strokeWidth: "25"
  },
  {
    /**
     * Top support
     */
    Type: Path,
    d: "M37.5 22.5 L245 22.5",
    strokeWidth: "25"
  },
  {
    /**
     * Force Leg Bar Join
     */
    Type: Path,
    d: "M40 97.5 L122.5 25",
    strokeWidth: "18"
  },
  {
    /**
     * Rope
     */
    Type: Path,
    d: "M247.5 10 L247.5 75",
    strokeWidth: "6"
  },
  {
    /**
     * The Head
     */
    Type: Circle,
    cx: "247.5",
    cy: "125",
    r: "50",
    fill: "transparent",
    strokeWidth: "4"
  },
  {
    /**
     * Body
     */
    Type: Path,
    d: "M247.5 175 L247.5 300",
    strokeWidth: "4"
  },
  {
    /**
     * Arm
     */
    Type: Path,
    d: "M247.5 220 L194.5 255",
    strokeWidth: "4"
  },
  {
    /**
     * Arm
     */
    Type: Path,
    d: "M247.5 220 L300.5 255",
    strokeWidth: "4"
  },
  {
    /**
     * leg
     */
    Type: Path,
    d: "M247.5 300 L300.5 335",
    strokeWidth: "4"
  },
  {
    /**
     * leg
     */
    Type: Path,
    d: "M247.5 300 L194.5 335",
    strokeWidth: "4"
  }
];

/**
 * Component to draw Hangman SVG
 * @param param0 @see Props
 * @returns The Hangman Draw Component
 */
function Hangman({
  width,
  height,
  quantity,
  color,
  backgroundColor
}: Props): JSX.Element {
  return (
    <svg
      version="1.1"
      viewBox="0 0 500 500"
      width={String(width)}
      height={String(height)}
      xmlns="http://www.w3.org/2000/svg"
    >
      <rect
        fill={backgroundColor || "transparent"}
        width={String(width)}
        height={String(height)}
      />
      {parts.slice(0, quantity).map((value: IPartProps, index: number) => {
        const { Type, ...props } = value;
        if (index === quantity - 1) {
          return (
            <Type
              key={index}
              {...props}
              stroke={color || "#000000"}
              strokeDasharray="2000"
            >
              <animate
                calcMode="spline"
                attributeName="stroke-dashoffset"
                from="2000"
                to="0"
                begin="0s"
                dur="4s"
              />
            </Type>
          );
        } else {
          return <Type key={index} {...props} stroke={color || "#000000"} />;
        }
      })}
    </svg>
  );
}

export default Hangman;
