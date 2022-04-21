import React from "react";
import { render, screen } from "@testing-library/react";
import Error404 from "../src/components/pages/Errors/Error404";

test("renders error page", () => {
  render(<Error404 />);
  const linkElement = screen.getByText(/Error/i);
  expect(linkElement).toBeInTheDocument();
});
