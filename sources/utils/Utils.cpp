/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:34:43 by pmelo-ca          #+#    #+#             */
/*   Updated: 2025/07/29 10:34:44 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils/Utils.hpp"

namespace Utils {
  std::string intToString(int value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
  }
}
